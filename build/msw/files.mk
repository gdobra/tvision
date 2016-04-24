BASECPPS = COLORSEL.cpp DRIVERS.cpp  DRIVERS2.cpp EDITSTAT.cpp \
           GRP.cpp      HARDWRVR.cpp HELP.cpp     HELPBASE.cpp \
           HISTLIST.cpp MAPCOLOR.cpp MENU.cpp     MISC.cpp     \
           MSGBOX.cpp   NEW.cpp      NEWSTR.cpp   PALETTE.cpp  \
           STDDLG.cpp   SYSERR.cpp   TAPPLICA.cpp TBKGRND.cpp  \
           TBUTTON.cpp  TCHDRDLG.cpp TCHECKBO.cpp TCLUSTER.cpp \
           TCMDSET.cpp  TCOLLECT.cpp TDESKTOP.cpp TDIALOG.cpp  \
           TDIRCOLL.cpp TDIRLIST.cpp TEDITOR1.cpp TEDITOR2.cpp \
           TEDITWND.cpp TEVENT.cpp   TEXTVIEW.cpp TFILDLG.cpp  \
           TFILECOL.cpp TFILEDTR.cpp TFILLIST.cpp TFRAME.cpp   \
           TGROUP.cpp   THISTORY.cpp THISTWIN.cpp THSTVIEW.cpp \
           TINDICTR.cpp TINPUTLI.cpp TLABEL.cpp   TLISTBOX.cpp \
           TLSTVIEW.cpp TMEMO.cpp    TMENUBAR.cpp TMENUBOX.cpp \
           TMNUVIEW.cpp TMOUSE.cpp   TMULCHKB.cpp TOBJECT.cpp  \
           TOBJSTRM.cpp TOUTLINE.cpp TPARAMTE.cpp TPOINT.cpp   \
           TPROGRAM.cpp TRADIOBU.cpp TRESCOLL.cpp TRESFILE.cpp \
           TSCREEN.cpp  TSCRLBAR.cpp TSCROLLE.cpp TSORTCOL.cpp \
           TSTATICT.cpp TSTATUSL.cpp TSTRCOLL.cpp TSTRLIST.cpp \
           TVALIDAT.cpp TVIEW.cpp    TVTEXT1.cpp  TVTEXT2.cpp  \
           TWINDOW.cpp  STRMSTAT.cpp tvwrite.cpp  tvcursor.cpp \
	   ttprvlns.cpp framelin.cpp edits.cpp    tgrmv.cpp    \
	   tvexposd.cpp file.cpp     

CPPS = $(BASECPPS)
OBJS = $(CPPS:.cpp=.obj)
