#!/usr/bin/env tclsh

# CreateNext4Bytes --
#
# Formats 4 bytes for a bitstream SPI or BPIx16
#
# Arguments:
# bitswap (optional) - Swap the bits in the byte
# 
# Results:
# return a single byte 
proc CreateNext4Bytes {word {bitswap 0} {byteswap 0}} {

    set return_string ""

    # puts "DEBUG: Next hex string : $word"


    switch $byteswap {
        0 {
            set byte_sequence {0 2 4 6}
        }
        1 {
            set byte_sequence {2 0 6 4}
        }
        default {
            error "Unknown byte swap pattern: $byteswap"
        }
    }

    foreach i $byte_sequence {

        set out_data [binary format H2 [string range $word $i [expr $i + 1]]]
        binary scan $out_data B8 bin_string

        # puts "DEBUG: Next byte : $bin_string"

        if {$bitswap} {
            append return_string [string reverse $bin_string]
        } else {
            append return_string $bin_string
        }

    }

    # puts "DEBUG: Next string: $return_string"
    return $return_string 
}


# CreateTimers --
#
# Create the BIN files for the Timers
#
# Arguments:
# timer - either 1 or 2 for the timer to be built
# bitswap (optional) - Swap the bits in each byte
# byteswap (optional) - Swap the bytes (every other byte)
# 
# Results:
# timer1.bin and timer2.bin are created.
#
# timer1.bin will be 1024 bytes with the bus width detect, sync word and timer
# values near the end of the .bin file.
#
# timer2.bin will be just over 256 bytes (includes 256 bytes of dummy words
# before the sync word
proc CreateTimers {timer {bitswap 0} {byteswap 0}} {

#    set timer1_size 1024
    #
    set dummy_word            "FFFFFFFF"
    set sync_word             "AA995566"
    set bus_width_detect_word "000000BB"
    set bus_width_word        "11220044"
    set noop_word             "20000000"
    set timer_word            "30022001"
    set timer_set_value [expr int(0x40000000)]
    # puts "DEBUG(CreateTimers): timer_set_value: $timer_set_value"

    if {$timer == "1"} {
        set timer_filename timer1.bin
        set header_size_words 242
        set timer_count 0x4000
    } else {
        set timer_filename "timer${timer}.bin"
        set header_size_words 1
        set timer_count 1
    }
    puts "Writing Timer: $timer_filename"

    set timer_list ""

    # Add the dummy words
    set i 0
    while {$i < $header_size_words} {
        incr i
        lappend timer_list $dummy_word
    }

    # Here's the main content of the timer image
    lappend timer_list $bus_width_detect_word
    lappend timer_list $bus_width_word
    lappend timer_list $dummy_word
    lappend timer_list $dummy_word
    lappend timer_list $sync_word
    lappend timer_list $noop_word
    lappend timer_list $noop_word
    lappend timer_list $noop_word
    lappend timer_list $timer_word

    # Figure out the timer string
    set timer_string [format "%08X" [expr $timer_set_value | $timer_count]]
    lappend timer_list $timer_string

    # Add a few dummy words and no-ops
    lappend timer_list $noop_word
    lappend timer_list $noop_word
    lappend timer_list $dummy_word
    lappend timer_list $dummy_word


	set fp [open $timer_filename w]
	fconfigure $fp -translation binary

    foreach i $timer_list {
        set next_word [CreateNext4Bytes $i $bitswap $byteswap]
        puts -nonewline $fp [binary format B32 $next_word]
    }

    close $fp
}



## ---- Main (script entry) ----

# Create the timer images
set bitswap 0
set byteswap 0
puts ""
CreateTimers 1 $bitswap $byteswap
CreateTimers 2 $bitswap $byteswap

exit



