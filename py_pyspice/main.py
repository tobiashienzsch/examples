
import math
import sys

import numpy as np
import matplotlib.pyplot as plt


import PySpice
from PySpice.Unit import u_kOhm, u_uF, u_V, u_Hz, u_kHz
from PySpice.Spice.Netlist import Circuit
from PySpice.Plot.BodeDiagram import bode_diagram

import PySpice.Logging.Logging as Logging
logger = Logging.setup_logging(logging_level='WARNING')


if sys.platform == 'linux' or sys.platform == 'linux2':
    PySpice.Spice.Simulation.CircuitSimulator.DEFAULT_SIMULATOR = 'ngspice-subprocess'
else:
    pass


circuit = Circuit('Low-Pass RC Filter')

circuit.SinusoidalVoltageSource('input', 'in', circuit.gnd, amplitude=2@u_V)
R1 = circuit.R(1, 'in', 'out', 1@u_kOhm)
C1 = circuit.C(1, 'out', circuit.gnd, 0.5@u_uF)


break_frequency = 1 / (2 * math.pi * float(R1.resistance * C1.capacitance))
print("Break frequency = {:.1f} Hz".format(break_frequency))

simulator = circuit.simulator(temperature=25, nominal_temperature=25)
analysis = simulator.ac(
    start_frequency=1@u_Hz,
    stop_frequency=22000@u_kHz,
    number_of_points=32,
    variation='dec'
)

figure, axes = plt.subplots(2, figsize=(20, 10))
plt.title("Bode Diagram of a Low-Pass RC Filter")
bode_diagram(
    axes=axes,
    frequency=analysis.frequency,
    gain=20*np.log10(np.absolute(analysis.out)),
    phase=np.angle(analysis.out, deg=False),
    marker='.',
    color='blue',
    linestyle='-',
)
for ax in axes:
    ax.axvline(x=break_frequency, color='red')

plt.tight_layout()
plt.show()
