### USERS SHOULD NOT NEED TO EDIT THIS FILE ###
# See Makefile in Examples directory for variable definitions

BINDIR = /home/usw/Install/MRnet/mrnet_4.1.0/build/x86_64-unknown-linux-gnu/bin
LIBDIR = /home/usw/Install/MRnet/mrnet_4.1.0/build/x86_64-unknown-linux-gnu/lib
TARGETS = $(BINDIR)/MRNetProdCon_FE \
          $(BINDIR)/MRNetProdCon_BE \

TARGETS_SO = $(LIBDIR)/MRNetProdConFilter.so

CCFLAGS = -O2 -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -Darch_x86_64 -Dcompiler_gnu -fPIC -Dos_linux -I/home/usw/Install/MRnet/mrnet_4.1.0/include -I/home/usw/Install/MRnet/mrnet_4.1.0/xplat/include -I/home/usw/Install/MRnet/mrnet_4.1.0/build/x86_64-unknown-linux-gnu -I/home/usw/Install/MRnet/mrnet_4.1.0/external/boost/include

CXXFLAGS = -O2 -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -Darch_x86_64 -Dcompiler_gnu -fno-default-inline -fPIC -Dos_linux -I/home/usw/Install/MRnet/mrnet_4.1.0/include -I/home/usw/Install/MRnet/mrnet_4.1.0/xplat/include -I/home/usw/Install/MRnet/mrnet_4.1.0/build/x86_64-unknown-linux-gnu -I/home/usw/Install/MRnet/mrnet_4.1.0/external/boost/include -Wall -Wno-system-headers -Wfloat-equal -Wconversion -Wshadow -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wsign-compare -Wredundant-decls -Wlong-long

LDFLAGS = -Wl,-E

SOFLAGS = -fPIC -shared -rdynamic

LIBS = -L/home/usw/Install/MRnet/mrnet_4.1.0/build/x86_64-unknown-linux-gnu/lib -lmrnet -lxplat -lm -lpthread -ldl

INSTALL = /usr/bin/install -c

ILIBDIR = //usr/local/lib

IBINDIR =//usr/local/bin


default: all

.PHONY: all clean install

all: $(TARGETS) $(TARGETS_SO)

$(BINDIR)/MRNetProdCon_FE: MRNetProdCon_FE.C
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ MRNetProdCon_FE.C $(LIBS)

$(BINDIR)/MRNetProdCon_BE: MRNetProdCon_BE.C
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ MRNetProdCon_BE.C $(LIBS)

$(LIBDIR)/MRNetProdConFilter.so: MRNetProdConFilter.C MRNetProdConIterator.C MRNetProdConGenericMerger.C MRNetProdConMerger.C MRNetProdConProducer.C MRNetThreads.C
	$(CXX) $(CXXFLAGS) $(SOFLAGS) -o $@ MRNetProdConFilter.C MRNetProdConIterator.C MRNetProdConGenericMerger.C MRNetProdConMerger.C MRNetProdConProducer.C MRNetThreads.C

clean:
	$(RM) -f $(TARGETS) $(TARGETS_SO)
	@echo "ECHOING PARAMS [start]"
	@echo "ECHOING PARAMS -------- dirs - BINDIR ; LIBDIR "
	@echo $(BINDIR)
	@echo $(LIBDIR)
	@echo "ECHOING PARAMS -------- flags -  CXX ; CCFLAGS;LDFLAGS;SOFLAGS "
	@echo $(CXX)
	@echo $(CCFLAGS)
	@echo $(CXXFLAGS)
	@echo $(LDFLAGS)
	@echo $(SOFLAGS)
	@echo "ECHOING PARAMS -------- libs - LIBS"
	@echo $(LIBS)
	@echo "ECHOING PARAMS -------- install - INSTALL"
	@echo $(INSTALL)
	@echo "ECHOING PARAMS -------- install dirs - ILIBDIR;IBINDIR"
	@echo $(ILIBDIR)
	@echo $(IBINDIR)
	@echo "ECHOING PARAMS [end]";

install: $(TARGETS) $(TARGETS_SO)
	for libso in $(TARGETS_SO) ; do \
	    $(INSTALL) -m 755 $$libso $(ILIBDIR)/ ; \
	done
	for exe in $(TARGETS) ; do \
	    $(INSTALL) -m 755 $$exe $(IBINDIR)/ ; \
	done
