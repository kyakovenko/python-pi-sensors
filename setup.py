from distutils.core import setup, Extension

cpp_module = Extension('dht11', 
                       libraries = ['wiringPi'],
                       sources=['dht11.c'],
                      )


setup(name='dht11', version='0.1',  \
      ext_modules=[cpp_module])
