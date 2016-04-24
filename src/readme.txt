The S*.CPP and NM*.CPP files have been removed and their content has been merged with the corresponding module;
This is due to the fact that when linked in a static library, the creation of the R* variables won't take place
because they are not referenced. Therefore, they will not be linked in, resulting in the pstream::types not being
populated with them. 

