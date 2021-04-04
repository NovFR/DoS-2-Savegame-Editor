
#define VERSION_MAJOR		1
#define REVISION_MAJOR		1
#define VERSION_MINOR		0
#define REVISION_MINOR		4

#define TOSTRINGTMP(x)		#x
#define TOSTRING(x)		TOSTRINGTMP(x)

#define VERSION_STRING_NZ	TOSTRING(VERSION_MAJOR) "." TOSTRING(REVISION_MAJOR) "." TOSTRING(VERSION_MINOR) "." TOSTRING(REVISION_MINOR) "\0"

#define TOWIDECHARTMP(x)	L ## x
#define TOWIDECHAR(x)		TOWIDECHARTMP(x)

#define VERSION_DATE		__DATE__ "\0"
#define VERSION_TIME		__TIME__ "\0"
