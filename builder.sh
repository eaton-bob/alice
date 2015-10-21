#!/bin/sh

make clean || true
make distclean || true

# This assumes the implicit origin is dev's fork
# and upstream is common source
if git pull --all ; then
	git merge upstream/master || exit $?
fi

(which gsl >/dev/null 2>&1) && \
	gsl project.xml

./autogen.sh && \
./configure && \
echo "=== MAKING PAR" && \
if make -j -k V=0 ; then : ; else
    echo "=== REMAKING SEQ"
    make V=1
fi

