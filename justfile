build:
    pip install --no-build-isolation -ve .

install:
    pip install .

dev-install-requirements:
    pip install nanobind scikit-build-core[pyproject]

