#----------------------------------------------------------------------
# proc show_file_dialog { w }
#----------------------------------------------------------------------

proc show_file_dialog { w } {
    
    fd_fill_listbox $w .
    wm deiconify $w
    grab $w
}

#----------------------------------------------------------------------
# proc create_file_dialog
#----------------------------------------------------------------------

proc create_file_dialog { name sel_command } {

    toplevel $name 

    label $name.label -text "Files/Directories:" -pady 3

    frame $name.main
    listbox $name.main.files -selectmode browse \
	    -yscrollcommand "$name.main.scroll set"
    scrollbar $name.main.scroll -command "$name.main.files yview"

    frame $name.input
    label $name.input.label -text "File: "
    entry $name.input.entry -width 30

    frame $name.buttons
    button $name.buttons.ok -text "Ok"  -command "fd_ok $name $sel_command"
    button $name.buttons.cancel -text "Cancel" \
	    -command "wm withdraw $name; grab release $name"
    pack $name.label -anchor w -padx 5

    pack $name.main -fill both -expand 1 -padx 5
    pack $name.main.files -side left -expand 1 -fill both
    pack $name.main.scroll -side left -fill y

    pack $name.input -fill x -pady 5 -padx 5
    pack $name.input.label -side left
    pack $name.input.entry -side left -expand 1 -fill x -padx 5

    pack $name.buttons -fill x -pady 10 -padx 5
    pack $name.buttons.ok -side left -expand 1
    pack $name.buttons.cancel -side left -expand 1

    bind $name.input.entry <Return> "
	eval [$name.buttons.ok cget -command]
    "
    bind $name.main.files <Double-Button-1> {
	set name %W
	set parent [string range "$name" 0 \
		[expr [string last ".main.files" "$name"]-1]]
	if {[llength [$name curselection]] > 0} {
	    $parent.input.entry delete 0 end
	    set name [$name get [lindex [$name curselection] 0]]
	    $parent.input.entry insert 0 "[exec pwd]/$name"
	    eval [$parent.buttons.ok cget -command]
	}
    }
	
    fd_fill_listbox $name .
    focus $name.main.files
    wm withdraw $name

    return $name
}

#----------------------------------------------------------------------
# proc fd_ok
#----------------------------------------------------------------------

proc fd_ok { w sel_command } {
    
    set path [$w.input.entry get]
    if [file isdirectory $path] {
	fd_fill_listbox $w $path
    } else {
	eval $sel_command [$w.input.entry get]
	grab release $w
	wm withdraw $w
    }
}

#----------------------------------------------------------------------
# proc fd_fill_listbox { w }
#----------------------------------------------------------------------

proc fd_fill_listbox { w path } {

    $w.main.files delete 0 end
    
    if [file isdirectory $path] {
	cd $path
	$w.main.files insert end "." ".." 
	set files [exec ls $path/]
	foreach i $files {
	    if [file isdirectory $path/$i] {
		$w.main.files insert end $i/
	    } else {
		$w.main.files insert end $i
	    }
	}
	$w.main.files activate 0
	$w.input.entry delete 0 end
	$w.input.entry insert 0 [exec pwd]/
    }	
}

#----------------------------------------------------------------------
#create_file_dialog .dir puts
