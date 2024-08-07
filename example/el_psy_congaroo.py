from llvmpym import error_handling
error_handling.install_fatal_error_handler(lambda s: print(s))
