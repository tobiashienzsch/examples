import os
import shutil
import subprocess

from jinja2 import Environment, FileSystemLoader, Template


def get_cc_compiler():
    cc = os.getenv('CC')
    if not cc:
        return 'cc'
    return cc


def get_cxx_compiler():
    cxx = os.getenv('CXX')
    if not cxx:
        return 'c++'
    return cxx


def clean_dir(path):
    try:
        shutil.rmtree(path)
    except OSError:
        print(f"Deletion of the directory {path} failed")


def create_dir(path):
    try:
        os.mkdir(path)
    except OSError:
        print(f"Creation of the directory {path} failed")


def which(exes):
    for exe in exes:
        print(subprocess.run(['which', exe], capture_output=True))


def copy_source_files(cwd, output, files):
    for file in files:
        shutil.copyfile(f"{cwd}/templates/{file}", f"{output}/{file}")


def render_template(env, output, template, arguments):
    t = env.get_template(f"{template}.jinja2")
    with open(f"{output}/{template}", 'w') as f:
        f.write(t.render(arguments))


def run_command(cmd):
    result = subprocess.run(cmd, capture_output=True)
    if result.returncode != 0:
        print(result.stdout)
        print(result.stderr)


def main():
    cwd = os.getcwd()
    project = 'drone'
    output = f"{cwd}/.sim"
    # print(f"The current working directory is {cwd}")

    # which(['cmake', 'ninja', 'make'])

    clean_dir(output)
    create_dir(output)

    copy_files = ["calc.hpp", "calc.cpp"]
    copy_source_files(cwd, output, copy_files)

    # write source
    env = Environment(loader=FileSystemLoader(f"{cwd}/templates"))
    args = {'project': project, 'files': copy_files, 'main': 'main.cpp'}
    render_template(env, output, 'CMakeLists.txt', args)
    render_template(env, output, 'main.cpp', {'return_code': '0'})

    # run cmake
    cmake_build_dir = f"{output}/build"

    cmake_config = ["cmake"]
    cmake_config.append(f"-H{output}")
    cmake_config.append(f"-B{cmake_build_dir}")
    cmake_config.append("-GNinja")
    cmake_config.append("-DCMAKE_BUILD_TYPE=Release")
    run_command(cmake_config)

    cmake_build = ["cmake"]
    cmake_build.append("--build")
    cmake_build.append(f"{cmake_build_dir}")
    run_command(cmake_build)

    # run simulation
    sim = [f"{cmake_build_dir}/{project}"]
    run_command(sim)


if __name__ == "__main__":
    main()
