from distutils.core import setup, Extension

cpp_module = Extension('dht', 
                       libraries = ['wiringPi'],
                       sources=['dht.c'],
                      )


setup(name='pisensors', version='0.1',  \
      ext_modules=[cpp_module])
