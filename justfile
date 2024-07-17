install:
    pip install --no-build-isolation -ve .

test:
    python -m pytest

build-wheels:
    python -m build
    
install-dev-requirements:
    pip install nanobind scikit-build-core[pyproject] pytest build

