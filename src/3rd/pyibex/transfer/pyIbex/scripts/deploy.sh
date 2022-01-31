#!/bin/bash -v


if [ $TRAVIS_OS_NAME == osx ]; then
  python -m twine upload --skip-existing -u ${PYPI_USERNAME} -p ${PYPI_PASSWORD} $TRAVIS_BUILD_DIR/${wheel_file};
fi

if [ $TRAVIS_OS_NAME == linux ]; then
  auditwheel repair $TRAVIS_BUILD_DIR/${wheel_file}
  twine upload --skip-existing -u ${PYPI_USERNAME} -p ${PYPI_PASSWORD} wheelhouse/*;
fi
