from setuptools import setup

with open("requirements.txt", "r", encoding="utf-8") as fh:
    requirements = fh.read()

setup(
    name='skylift',
    version='0.1.0',    
    description='Geolocation spoofing research',
    url='https://github.com/adamhrv/skylift',
    author='Adam Harvey',
    license='MIT',
    py_modules=['src'],
    packages=['src'],
    install_requires=[requirements],
    python_requires='>=3.7',
    classifiers=[
        'Development Status :: 4 - Beta',
        'License :: OSI Approved :: MIT License'
    ],
     entry_points = {
        'console_scripts': ['skylift=src.cli:cli'],
    }
)