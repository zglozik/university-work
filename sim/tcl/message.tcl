
#----------------------------------------------------------------------
# Widgets to print messages
#----------------------------------------------------------------------

proc create_message {} {

    toplevel .message
    message .message.text -width 150
    button .message.ok -text "Ok" -command "wm withdraw .message"

    pack .message.text -fill both -expand 1 -padx 20 -pady 10
    pack .message.ok -pady 5 

    wm withdraw .message
}

proc print_message { msg } {

    .message.text configure -text $msg 
    wm deiconify .message
}
