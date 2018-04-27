%module example
 %{
 /* Includes the header in the wrapper code */
 #include "derecho/derecho.h"
 %}
 
 /* Parse the header file to generate wrappers */
 %include "derecho/derecho.h"
 
 %template(RawGroup) derecho::Group<>

 extern derecho::DerechoParams
 extern derecho::SubgroupInfo
 extern RawGroup
 extern derecho::RawSubgroup
 extern derecho::RawObject
 
 
 