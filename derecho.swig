%module example
 %{
 /* Includes the header in the wrapper code */
 #include "derecho/derecho.h"
 %}
 
 /* Parse the header file to generate wrappers */
 %include "derecho/derecho.h"