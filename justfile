set dotenv-load

install:
    pip install --no-build-isolation -ve .

test: install-normal
    python -m pytest

build-wheels: install-normal
    python -m build

ipython: install-normal
    ipython

install-normal:
    pip install .
    
install-dev-requirements:
    pip install nanobind scikit-build-core[pyproject] pytest build


