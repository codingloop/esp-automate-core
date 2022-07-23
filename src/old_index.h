const char MAIN_page[] PROGMEM = R"(
<HTML>
	<HEAD>
			<TITLE>Smart Home Automation</TITLE>
	</HEAD>
<BODY>
	<CENTER>
			<h1>Smart Home Automation Web Control</h1><br><br>
<p><b>Built-In LED #1 </b><a href="switching?c1=2&c2=0"><button>ON</button></a>&nbsp;<a href="switching?c1=2&c2=1"><button>OFF</button></a></p>
<p><b>Light-1 (D2)&nbsp;&nbsp; #2 </b><a href="switching?c1=4&c2=1"><button>ON</button></a>&nbsp;<a href="switching?c1=4&c2=0"><button>OFF</button></a></p>
<br><br><p><b>Fan Controls</b></p>
<p>
<a href="speed?dl1=0"><button>OFF</button></a>&nbsp;
<a href="speed?dl1=6"><button>1</button></a>&nbsp;
<a href="speed?dl1=5"><button>2</button></a>&nbsp;
<a href="speed?dl1=4"><button>3</button></a>&nbsp;
<a href="speed?dl1=3"><button>4</button></a>&nbsp;
<a href="speed?dl1=2"><button>5</button></a>&nbsp;
<a href="speed?dl1=1"><button>6</button></a>&nbsp;
<a href="speed?dl1=1"><button>ON</button></a>
</p>
  </CENTER> 
</BODY>
</HTML>
)";