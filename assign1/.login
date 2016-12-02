stty -istrip
#stty erase '^H' # Remove first # if backspace doesn't work properly

setenv NNTPSERVER csnews.cs.utsarr.net

if ( $TERM != "xterm" ) then
	if ( -f "/usr/bin/test" ) then
		set noglob
		eval `/usr/bin/tset -s -r -m :?xterm`
		unset noglob
	endif
endif

/usr/bin/rtin -v -z

last | head
echo ''
w	# see who is logged in
echo ''
