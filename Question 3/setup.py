from setuptools import setup, Extension

# Define the C extension
sensor_extension = Extension(
    'sensormodule',              # Name of the module
    sources=['sensor_module.c']  # Source file
)

setup(
    name='SensorModule',
    version='1.0',
    description='Python extension for IoT sensors',
    ext_modules=[sensor_extension] # List of extensions to build
)
