
#include	"viewer.hh"

class Application {

//	ctors & dtor

    public:
	Application( const char *name );
	virtual ~Application();

//	opers

    public:
	Widget	base()	{ return base_widget; }

//	private data

    private:
	static	void	main_loop( Widget w, XtPointer cld, XtPointer cad );

	Widget	base_widget;
};

// ----------------------------------------------------------------------
//	Application class
// ----------------------------------------------------------------------

Application::Application( const char * name )
{
	base_widget = XtInitialize( name, "simulator", NULL, 0, NULL, NULL );
	XtRealizeWidget( base_widget );
	XtAddWorkProcedure
}

Application::~Application()
{
	XtDestroyWidget( base_widget );
}

void Application::main_loop( Widget w, XtPointer cld, XtPointer cad )
{

}

// ----------------------------------------------------------------------
//	XViewer class
// ----------------------------------------------------------------------

XViewer::XViewer( const char *name )
{

}

XViewer::~XViewer()
{
	XtDestroyWidget( window );
}

Viewer &XViewer::move_to( double x, double y, double z )
{

	return *this;
}

Viewer &XViewer::line_to( double x, double y, double z )
{

	return *this;
}
