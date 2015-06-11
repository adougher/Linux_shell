# To build the shell (which is called MyShell by default), simply type:
#   make
#
# To clean up and remove the compiled binary, type:
#   make clean
#
# If you doing development on a Mac, then be aware that Mac OS X has a different
# version of the readline library than Alamode. 
#
# To compile on a Mac, follow these instructions:
#   - Install readline from source. I used Homebrew (http://brew.sh):
#        brew install readline
#   - Take note of where Homebrew says it wrote the lib and include folders.
#     Update MAC_LDFLAGS and MAC_CPPFLAGS if your locations are different.
#   - Then type:
#        make mac
#
# WARNING: You may develop your shell on any machine you want, but it *MUST*
# compile and run on the Alamode lab computers!
#
# If you have trouble compiling, feel free to post a question on Piazza.

OBJS = shell.cpp auto_completion.cpp did_you_mean.cpp builtins.cpp common.cpp cmd_execution.cpp
NAME = MyShell
CPP_FLAGS = -Wall -l readline
MAC_LDFLAGS = -L/usr/local/homebrew/opt/readline/lib
MAC_CPPFLAGS = $(CPP_FLAGS) -I/usr/local/homebrew/opt/readline/include

all: $(NAME)

MyShell: $(OBJS)
	g++ $(OBJS) -o $(NAME) $(CPP_FLAGS)

debug: $(OBJS)
	g++ $(OBJS) -o $(NAME) $(CPP_FLAGS) -g

mac: $(OBJS)
	g++ $(OBJS) -o $(NAME) $(MAC_CPPFLAGS) $(MAC_LDFLAGS)

mac_debug: $(OBJS)
	g++ $(OBJS) -o $(NAME) $(MAC_CPPFLAGS) -g $(MAC_LDFLAGS)

clean:
	rm -rf $(NAME)*
