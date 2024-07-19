install:
    pip install --no-build-isolation -ve .

test: install-normal
    python -m pytest

install-normal:
    pip install .

build-wheels:
    python -m build
    
install-dev-requirements:
    pip install nanobind scikit-build-core[pyproject] pytest build

