
#----------------------------------------------------------------------
# initalize module
#----------------------------------------------------------------------

set WIDTH 0
set HEIGHT 0
set OX 0
set OY 0
set AWIDTH 0
set AHEIGHT 0
set XBEGIN 0
set XEND 0
set YBEGIN 0
set YEND 0

set colors(0) "violet"
set colors(1) "dark orchid"
set colors(2) "forest green"
set colors(3) "seashell4"
set colors(4) "aquamarine"
set colors(5) "sea green"
set colors(6) "gold"
set colors(7) "azure4"
set colors(8) "turquoise4"
set colors(9) "alice blue"
set colors(10) "VioletRed4"
set colors(11) "IndianRed4"
set colors(12) "bisque"
set colors(13) "misty rose"
set colors(14) "moccasin"

#----------------------------------------------------------------------
# procedures for handling diagrams
#----------------------------------------------------------------------

#----------------------------------------------------------------------
# proc show_diagram { widget minx maxx data header }
#----------------------------------------------------------------------

proc show_diagram { widget minx maxx data header } {

    global WIDTH HEIGHT OX OY AWIDTH AHEIGHT
    set limits [compute_limits $data]
    set miny [lindex $limits 0]
    set maxy [lindex $limits 1]
    set WIDTH [$widget cget -width]
    set HEIGHT [$widget cget -height]
    set OX 90
    set OY [expr $HEIGHT-90]
    set AWIDTH [expr $WIDTH-2*$OX]
    set AHEIGHT [expr $OY-($HEIGHT-$OY)]

    $widget delete all
    show_axes $widget $minx $maxx $miny $maxy
    show_values $widget $minx $maxx $miny $maxy $data $header
}

#----------------------------------------------------------------------
# proc show_axes { widget minx maxx miny maxy }
#----------------------------------------------------------------------

proc show_axes { widget minx maxx miny maxy } {

    global WIDTH HEIGHT OX OY AWIDTH AHEIGHT XBEGIN XEND YBEGIN YEND
    $widget create line $OX $OY [expr $OX+$AWIDTH] $OY
    $widget create line $OX $OY $OX [expr $OY-$AHEIGHT]

    if {($maxx-$minx)/0.1 > 10} {
	set xstep 1
    } else {
	set xstep 0.1
    }
    set hwidth [expr $AWIDTH / (($maxx-$minx) / $xstep + 2.0)]
    set vwidth [expr $AHEIGHT / 12.0]

    set rx [expr $OX + $hwidth]
    set XBEGIN $rx
    for {set x $minx} {$x <= $maxx} { set x [expr $x + $xstep]} {
	$widget create line $rx $OY $rx [expr $OY-5] -width 2
	$widget create text $rx [expr $OY+3] -text [expr round($x*100)/100.0]\
		-anchor n
	set rx [expr $rx + $hwidth]
    }
    set XEND [expr $XBEGIN + ($maxx-$minx)*($rx-$XBEGIN) / ($x-$minx)]

    set ry [expr $OY - $vwidth]
    set YBEGIN $ry
    for {set i 0} {$i < 10} {incr i} {
	$widget create line $OX $ry [expr $OX+5] $ry -width 2
	$widget create text [expr $OX-3] $ry -text [expr round(100* \
		($miny + $i*($maxy-$miny)/9)) / 100.0] -anchor e
	set ry [expr $ry - $vwidth]
    }
    set YEND [expr $ry+$vwidth]

    $widget create text 20 [expr ($OY + $HEIGHT)/2] -anchor w -text "Item: " \
	    -tags text
}

#----------------------------------------------------------------------
# proc show_values { widget minx maxx miny maxy data header }
#----------------------------------------------------------------------

proc show_values { widget minx maxx miny maxy data header } {

    global colors

    for {set i 0} {$i < [llength $data]} {incr i} {
	show_value $widget $minx $maxx $miny $maxy [lindex $data $i] \
		$colors([expr $i % [array size colors]]) [lindex $header $i]
    }
}

#----------------------------------------------------------------------
# proc show_value { widget minx maxx miny maxy data color head }
#----------------------------------------------------------------------

proc show_value { widget minx maxx miny maxy data color head } {

    global WIDTH HEIGHT OX OY AWIDTH AHEIGHT XBEGIN XEND YBEGIN YEND

    set num_pts [llength $data]
    set vwidth [expr $YBEGIN - $YEND]
    for {set i 0} {$i < $num_pts} {incr i} {
	set x [expr $XBEGIN + $i * ($XEND-$XBEGIN) / ($num_pts-1)]
	set y [expr $YBEGIN - ([lindex $data $i]-$miny) * \
		$vwidth / ($maxy-$miny)]
	append line "$x $y "
    }
    eval $widget create line $line -width 3 -fill \"$color\" -tags $head
    $widget bind $head "<Enter> <Motion>" \
	    "enter_line $widget \"$head\" \"$data\" $minx $maxx %x"
    $widget bind $head <Leave> \
	    "$widget itemconfigure text -text \"Item: \""
    $widget bind $head <1> \
	    "show_position $widget \"$data\" $minx $maxx %x"
}

#----------------------------------------------------------------------
# proc enter_line { widget head data minx maxx x }
#----------------------------------------------------------------------

proc enter_line { widget head data minx maxx x } {
    global XBEGIN XEND
    
    set x [expr int([llength $data] * ($x-$XBEGIN) / ($XEND-$XBEGIN))]
    if { $x == [llength $data] } {
	incr x -1
    }
    set y [expr round([lindex $data $x] * 100) / 100.0]

    $widget itemconfigure text -text "Item: $head, [expr $x*0.01] --> $y"
}

#----------------------------------------------------------------------
# proc show_position { widget data minx maxx x }
#----------------------------------------------------------------------

proc show_position { widget data minx maxx x } {
    global XBEGIN XEND sim
    
    set x [expr int([llength $data] * ($x-$XBEGIN) / ($XEND-$XBEGIN))]
    if { $x == [llength $data] } {
	incr x -1
    }
    stop_cb $sim 0

    .frame3.start_entry delete 0 end
    .frame3.start_entry insert 0 $x
    .frame3.end_entry delete 0 end
    .frame3.end_entry insert 0 $x
    run_cb
}

#----------------------------------------------------------------------
# proc compute_limits { data }
#----------------------------------------------------------------------

proc compute_limits { data } {

    set miny [lindex [lindex $data 0] 0]
    set maxy $miny

    foreach column $data {
	foreach value $column {
	    if {$value > $maxy} {
		set maxy $value
	    }
	    if {$value < $miny} {
		set miny $value
	    }
	}
    }
    if {$miny == $maxy} {
	set maxy [expr $maxy + 1]
    }
    return "$miny $maxy"
}

#----------------------------------------------------------------------
# proc create_show_dialog { widget }
#----------------------------------------------------------------------

proc create_show_dialog { widget } {

    toplevel $widget
    
    frame $widget.panel
    frame $widget.panel.hide_panel
    frame $widget.panel.show_panel

    label $widget.panel.hide_panel.label -text "All data:"
    label $widget.panel.show_panel.label -text "Data to show:"

    listbox $widget.panel.hide_panel.hide_list -selectmode extended \
	    -yscrollcommand "$widget.panel.hide_panel.hide_sb set"
    listbox $widget.panel.show_panel.show_list -selectmode extended \
	    -yscrollcommand "$widget.panel.show_panel.show_sb set"

    scrollbar $widget.panel.hide_panel.hide_sb \
	    -command "$widget.panel.hide_panel.hide_list yview"
    scrollbar $widget.panel.show_panel.show_sb \
	    -command "$widget.panel.show_panel.show_list yview"

    frame $widget.panel.middle_buttons
    button $widget.panel.middle_buttons.add -text "Add -->"
    button $widget.panel.middle_buttons.remove -text "Remove <--"

    frame $widget.buttons
    button $widget.buttons.show -text "Show" -command "wm withdraw $widget"
    button $widget.buttons.cancel -text "Cancel" -command "wm withdraw $widget"

    pack $widget.panel -padx 10 -pady 10 -fill both -expand 1
    pack $widget.panel.hide_panel -side left -fill both -expand 1
    pack $widget.panel.hide_panel.label -anchor w
    pack $widget.panel.hide_panel.hide_list -side left -fill both -expand 1
    pack $widget.panel.hide_panel.hide_sb -side left -fill y

    pack $widget.panel.middle_buttons -side left -fill y
    pack $widget.panel.middle_buttons.add -expand 1 -pady 10
    pack $widget.panel.middle_buttons.remove -expand 1 -pady 10

    pack $widget.panel.show_panel -side left -fill both -expand 1
    pack $widget.panel.show_panel.label -anchor w
    pack $widget.panel.show_panel.show_list -side left -fill both -expand 1
    pack $widget.panel.show_panel.show_sb -side left -fill y

    pack $widget.buttons -fill x -pady 10
    pack $widget.buttons.show -side left -expand 1
    pack $widget.buttons.cancel -side left -expand 1

    bind $widget.panel.hide_panel.hide_list <Double-Button-1> \
	    "$widget.panel.middle_buttons.add invoke"
    bind $widget.panel.show_panel.show_list <Double-Button-1> \
	    "$widget.panel.middle_buttons.remove invoke"

    wm title $widget "Show data dialog"
    wm withdraw $widget
}

#----------------------------------------------------------------------
# proc move_columns { source destination store }
#----------------------------------------------------------------------

proc move_columns { source destination store } {

    set to_add [lsort -integer -decreasing [$source curselection]]
    set cols ""
    for {set i 0} {$i < [llength $to_add]} {incr i} {
	set curindex [lindex $to_add $i]
	lappend cols [lsearch -exact $store [$source get $curindex]]
	$source delete $curindex
    }

    set dcols [$destination get 0 end]
    foreach column $cols {
	set found 0
	for {set i 0} {$i < [llength $dcols] && ! $found} {incr i} {
	    set found [expr $column < \
	    	[lsearch -exact $store [lindex $dcols $i]]]
	}
	if {$found} {
	    incr i -1
	} else {
	    set i end
	}
	$destination insert $i [lindex $store $column]
	set dcols [linsert $dcols $i [lindex $store $column]]
    }
}

#----------------------------------------------------------------------
# update_show_dialog { widget header wdiagram mint maxt data }
#----------------------------------------------------------------------

proc update_show_dialog { widget header wdiagram mint maxt data } {

    $widget.panel.hide_panel.hide_list delete 0 end
    $widget.panel.show_panel.show_list delete 0 end
    eval $widget.panel.hide_panel.hide_list insert 0 $header

    $widget.panel.middle_buttons.add configure -command "move_columns \
    	    $widget.panel.hide_panel.hide_list \
	    $widget.panel.show_panel.show_list \"$header\""
    $widget.panel.middle_buttons.remove configure -command "move_columns \
    	    $widget.panel.show_panel.show_list \
	    $widget.panel.hide_panel.hide_list \"$header\""
    $widget.buttons.show configure \
	    -command "show_diagram_cb $widget \"$header\" \
	    $wdiagram $mint $maxt \"$data\""
}

#----------------------------------------------------------------------
# proc show_diagram_cb { widget header wdiagram mint maxt data }
#----------------------------------------------------------------------

proc show_diagram_cb { widget header wdiagram mint maxt data } {
    
    set sel [$widget.panel.show_panel.show_list get 0 end]

    if {[llength $sel] == 0} {
	print_message "You must select at least one column!"
	return
    }

    for {set i 0} {$i < [llength $sel]} {incr i} {
	set datai [lsearch -exact $header [lindex $sel $i]]
	lappend new_data [lindex $data $datai]
	lappend new_header [lindex $header $datai]
    }

    show_diagram $wdiagram.frame.canvas $mint $maxt $new_data $new_header
    wm deiconify $wdiagram
}
