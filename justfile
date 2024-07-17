install:
    pip install --no-build-isolation -ve .

install-requirements:
    pip install nanobind scikit-build-core[pyproject]
