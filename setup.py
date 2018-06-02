from distutils.core import setup, Extension

gauss1 = Extension('gauss',
                    sources = ['main.c','eliminate.c'])

setup (name = 'gauss',
       version = '1.0',
       description = 'Implementation of Gaussian elimination algorithm.',
ext_modules = [gauss1])
