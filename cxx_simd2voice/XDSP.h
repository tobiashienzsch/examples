#ifndef __XDSP_H__
#define __XDSP_H__

// File: XDSP.h
// Defines basic DSP graph processing types
// Author: Angus F. Hewlett
// Copyright FXpansion Audio UK Ltd. 2001-2017

// This file provides:-
// * ProcessGlobals - an object representing shared processing variables: block
// size, tempo, sample rate etc.
// * SIMDLevel - report supported SIMD feature set
// * XDSPNode, XDSPVoice - base interface for a signal processing node & its
// voices
// * XDSPNodeTmpl, XDSPVoiceTmpl - CRTP default implementation for type-specific
// node/voice functionality
// * JobQueue, Job - queue of processing jobs (to support async operation)

#include <stdint.h>
#include <vector>

#include "XHelpers.h"

namespace XDSP
{
static constexpr int32 kMaxVoices       = 64;
static constexpr int32 kMaxControlPorts = 64;
static constexpr int32 kMaxAudioPorts   = 8;

// Data shared by all nodes in a processing graph: sample rate, buffer size,
// voice count etc.
class ProcessGlobals
{
public:
    int32 block_length      = 64;
    int32 block_length_norm = block_length;
    float analog_noise[2];
    float srscale               = 1.f;
    float block_length_norm_inv = 1.f / block_length;
    float uncorr                = 0.0000001f;
    float filtg_2_5;
    float tune            = 44100.f;
    float sample_rate     = 96000.f;
    float sample_rate_inv = 1.f / sample_rate;
    float* hum;
};

/// \brief SIMD feature level helper class
struct SIMDLevel
{
    enum SIMDFeatureLevel
    {
        eNoSIMD,
        eFeatSSE2,
        eFeatSSE4,
        eFeatAVX,
        eFeatFMA,
        eFeatAVX512,
    };

    /// \brief Report CPU feature level
    [[nodiscard]] static auto GetFeatureLevel() noexcept -> SIMDFeatureLevel
    {
        // Beyond the scope of this demo to support platform-independent CPU
        // feature detection. Pretend we're running on Haswell+
        return eFeatFMA;
    };
};

// A polyphonic filter class
class Node
{
public:
    // A voice on a specific instance of a filter
    class Voice
    {
    public:
        Voice() {};
        virtual ~Voice() {};
        float* ControlIn(int32 control_port_index)
        {
            return m_control_ins[control_port_index];
        };
        void SetControlIn(int32 control_port_index, float* ptr)
        {
            m_control_ins[control_port_index] = ptr;
        };
        const float* AudioIn(int32 audio_port_index)
        {
            return m_audio_ins[audio_port_index];
        };
        float* AudioOut(int32 audio_port_index)
        {
            return m_audio_outs[audio_port_index];
        };
        void SetAudioIn(int32 audio_port_index, float* buffer)
        {
            m_audio_ins[audio_port_index] = buffer;
        };
        void SetAudioOut(int32 audio_port_index, float* buffer)
        {
            m_audio_outs[audio_port_index] = buffer;
        };

        virtual void PrepareStream(const ProcessGlobals& process_globals) {};
        virtual void Reset() {};

    protected:
        float* m_control_ins[kMaxControlPorts];
        float* m_audio_ins[kMaxAudioPorts];
        float* m_audio_outs[kMaxAudioPorts];
    };

    // Empty default implementation for process attributes.
    class ProcessAttributes
    {
    };

    /*
     * Prepare for streaming.. allocate buffers, etc.
     */
    virtual void PrepareStream(const ProcessGlobals& process_globals)
    {
        for (int32 i = 0; i < kMaxVoices; i++)
            m_voices[i]->PrepareStream(process_globals);
    };

    /*
     * Entry point to processing, called from the graph-walker, dispatched by
     * NodeTmpl below.
     */
    virtual int32 ProcessBuffer_Dispatch(const ProcessGlobals& process_globals)
        = 0;

    /*
     * Allocate voices. Implemented @ NodeTmpl
     */
    virtual void AllocateVoices() = 0;

    virtual int32 AudioInCount()  = 0;
    virtual int32 AudioOutCount() = 0;

    /*
     * Accessors for voices.
     */
    std::vector<Voice*>& GetVoices() { return m_voices; };
    Voice* GetVoice(int32 voice) { return m_voices[voice]; };
    const ProcessAttributes& GetProcessAttributes() { return m_procAttr; };

protected:
    ProcessAttributes m_procAttr;
    std::vector<Voice*> m_voices;
};

// JobQueue - a typed queue of pending jobs for a particular processing object.
// Used for asynchronous processing only.
template<class TNode, class TWorker>
class JobQueue
{

public:
    typedef typename TNode::ProcessAttributes ProcessAttributes;

    static constexpr int32 kQSize      = XDSP::kMaxVoices / TWorker::num_elem;
    static constexpr int32 kNumThreads = 1;

    // Job - a job in the work queue
    class Job
    {
    public:
        typedef typename TWorker::VoiceBlock VoiceBlock;
        typedef typename TWorker::IOAdapterImpl IOAdapter;

        Job() {};
        Job(VoiceBlock& _voiceIO, const XDSP::ProcessGlobals& process_globals,
            const ProcessAttributes& _pa)
            : voiceIO(&_voiceIO)
            , procGlobals(&process_globals)
            , procAttr(&_pa) {};

        /*
         * Run - when run, the job will call the static ProcessBuffer function
         * for its worker type.
         */
        virtual void Run()
        {
            IOAdapter voice_io(*voiceIO);
            TWorker::ProcessBuffer(*procGlobals, voice_io, *procAttr);
        };

    protected:
        VoiceBlock* voiceIO               = 0;
        const ProcessGlobals* procGlobals = 0;
        const ProcessAttributes* procAttr = 0;
    };

protected:
    Job m_job_queue[kQSize > 0 ? kQSize : 1];
    int32 m_q_pos;

public:
    JobQueue() : m_q_pos(0) {};

    /*
     * Enqueue - enqueue a job
     */
    void Enqueue(const Job& job)
    {
        if (m_q_pos >= kQSize) return;
        m_job_queue[m_q_pos] = job;
        m_q_pos++;
    };

    /*
     * ProcessQueue - process queued jobs
     */
    void ProcessQueue()
    {
        if (!m_q_pos) return;  // No jobs - nothing to do.
        for (int32 i = 0; i < m_q_pos; i++)
        {
            m_job_queue[i].Run();
        }
        m_q_pos = 0;
    };
};

/////////////////////////
// This class will dispatch to the right method based on the available CPU
// features
/////////////////////////
template<class T>
class NodeTmpl : public Node
{
public:
    typedef typename T::Node TNode;
    typedef typename T::Voice TVoice;

    // x86 / x64 dispatch
    // These are compiled on all architectures but not called (they live in
    // their own compilation units)
#if ARCH_X86 || ARCH_X64
#if ENABLE_AVX512
#endif
#if ENABLE_AVX
    static void ProcessBuffer_AVX2(const ProcessGlobals& process_globals,
                                   TNode* caller);
    static void ProcessBuffer_AVX(const ProcessGlobals& process_globals,
                                  TNode* caller);
#endif
#if ENABLE_SSE
    static void ProcessBuffer_SSE4(const ProcessGlobals& process_globals,
                                   TNode* caller);
    static void ProcessBuffer_SSE2(const ProcessGlobals& process_globals,
                                   TNode* caller);
#endif
#endif
#if ARCH_ARM
    // ARM dispatch
    static int32 ProcessBuffer_NEON(const ProcessGlobals& process_globals,
                                    TNode* caller);
#endif

    // Scalar dispatch
    static int32 ProcessBuffer_Scalar(const ProcessGlobals& process_globals,
                                      TNode* caller);

    static constexpr bool s_enableAsyncProcess = false;
    static constexpr size_t s_mem_align_pow
        = 6;  // 2^6 = 64 bytes = 1 cache line
    static constexpr size_t s_mem_align = 1 << s_mem_align_pow;
    static constexpr size_t s_voice_size_padded
        = (((sizeof(TVoice) >> s_mem_align_pow) + 1) << s_mem_align_pow);

    //
    NodeTmpl() { AllocateVoices(); }

    //
    virtual ~NodeTmpl() { FreeVoices(); }

    // Allocate voices using placement new
    virtual void AllocateVoices()
    {
        m_voiceMemory = valigned_malloc(s_voice_size_padded * XDSP::kMaxVoices,
                                        s_mem_align);
        m_voices.resize(XDSP::kMaxVoices);
        // printf("Allocated %lu bytes (voice = %lu, aligned = %lu)\n",
        // s_voice_size_padded * XDSP::kMaxVoices, sizeof(TVoice),
        // s_voice_size_padded);
        for (int32 i = 0; i < XDSP::kMaxVoices; i++)
        {
            char* voiceMem = ((char*)m_voiceMemory) + (i * s_voice_size_padded);
            m_voices[i]    = new (voiceMem) TVoice();
        }
    };

    // Free voices using placement destruct
    virtual void FreeVoices()
    {
        for (int32 i = 0; i < XDSP::kMaxVoices; i++)
        {
            static_cast<TVoice*>(m_voices[i])->~TVoice();
        }
        valigned_free(m_voiceMemory);
    };

    virtual TVoice* GetVoiceTyped(int32 voice)
    {
        return static_cast<TVoice*>(m_voices[voice]);
    };

    virtual int32 AudioInCount() { return T::kNumAudioInputs; };

    virtual int32 AudioOutCount() { return T::kNumAudioOutputs; };

    // Default impl for buffer processing, just call ProcessBuffer_Dispatch().
    virtual void ProcessBuffer(const XDSP::ProcessGlobals& process_globals)
    {
        // Call the templated base class to dispatch
        ProcessBuffer_Dispatch(process_globals);
    }

    // Main dispatch entry point for buffer processing, selects which CPU
    // specific dispatch to use
    virtual int32 ProcessBuffer_Dispatch(const ProcessGlobals& process_globals)
    {
        //        int32 cpu_feature_level = SIMDLevel::GetFeatureLevel();

        ///////
        // These static functions ( NodeTmpl<T>::ProcessBuffer_$ARCH ) are
        // implemented as specializations by way of a CPU-specific (rather than
        // type specific) compilation unit. They in turn instantiate templates
        // for a specific node-type and mathops-implementation

#if ARCH_X86 || ARCH_X64
#if ENABLE_AVX512
        if (cpu_feature_level >= eFeatAVX512)
            return ProcessBuffer_AVX512(process_globals, this);
#endif
#if ENABLE_AVX
        if (cpu_feature_level >= SIMD::eFeatFMA)
            return ProcessBuffer_AVX2(process_globals,
                                      static_cast<TNode*>(this));
        if (cpu_feature_level >= SIMD::eFeatAVX)
            return ProcessBuffer_AVX(process_globals,
                                     static_cast<TNode*>(this));
#endif
#if ENABLE_SSE
        if (cpu_feature_level >= SIMD::eFeatSSE4)
            return ProcessBuffer_SSE4(process_globals,
                                      static_cast<TNode*>(this));
        if (cpu_feature_level >= SIMD::eFeatSSE2)
            return ProcessBuffer_SSE2(process_globals,
                                      static_cast<TNode*>(this));
#endif
#endif

#if ARCH_ARM && ENABLE_NEON
        if (cpu_feature_level >= SIMD::eFeatNEON)
            return T::ProcessBuffer_NEON(process_globals,
                                         static_cast<TNode*>(this));
#endif
        return ProcessBuffer_Scalar(process_globals, static_cast<TNode*>(this));
        return 0;
    }

    // Template for packing voices in to blocks and dispatching to a
    // CPU-specific worker
    template<class TWorker>
    static void ProcessAllVoices(const XDSP::ProcessGlobals& process_globals,
                                 TNode* node)
    {
        // PerformanceCounterScope p(__FUNCTION__);

        // Can check for no / few active voices here, node global power on/off
        // etc.
        typedef typename TWorker::VoiceBlock VoiceBlock;
        typedef typename XDSP::JobQueue<TNode, TWorker> JobQueueImpl;

        JobQueueImpl voice_block_queue;

        // Pack the voices and enqueue them for processing
        for (int32 i = 0; i < XDSP::kMaxVoices; i += TWorker::num_elem)
        {
            // local_voices contains a subset of the global voice array adapted
            // to the SIMD operation width
            VoiceBlock local_voices;
            for (int j = 0; j < TWorker::num_elem; j++)
            {
                typename T::Voice* voice
                    = static_cast<typename T::Voice*>(node->GetVoice(i + j));
                //  voice->PreProcessBuffer(process_globals);
                local_voices[j] = voice;
            }

            // Create an I/O adapter for this group of voices
            typename TWorker::IOAdapterImpl voice_io(local_voices);

            if (s_enableAsyncProcess)
            {
                // Create a job to call ProcessBuffer() with this voice I/O
                // adapter
                voice_block_queue.Enqueue(
                    typename JobQueueImpl::Job(local_voices, process_globals,
                                               node->GetProcessAttributes()));
            }
            else
            {
                // Process immediately
                TWorker::ProcessBuffer(process_globals, voice_io,
                                       node->GetProcessAttributes());
            }
        }
        // If async, process the queue once everything has been enqueued
        if (s_enableAsyncProcess)
        {
            voice_block_queue.ProcessQueue();
        }
        // The performance counter scope will register its output on destruct.
    };

protected:
    void* m_voiceMemory = 0;
};

// Template implementation class for node-instance-specific voice data
template<class T>
class VoiceTmpl : public Node::Voice
{
public:
};

};  // namespace XDSP

#endif  // __XDSP_H__
