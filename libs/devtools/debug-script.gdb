
define connect
	target remote | openocd -c 'debug_level 0' -f board/st_nucleo_f4.cfg -c 'gdb_port pipe' -l /dev/null 2> /dev/null
end

define simple-reset
       monitor reset halt
end

define reset
       simple-reset
       tbreak main
       continue
end

define hook-run
       reset
end

define hookpost-load
       reset
end

connect
reset
