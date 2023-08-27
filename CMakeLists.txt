cmake_minimum_required(VERSION 3.12)                                                                                                                                                                                                      
                                                                                                              
project(gcd_web)                                                                                    
                                                                                                              
# generate compile_commands.json; used by YCM                                                                 
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)                                                                         
set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})

set(CMAKE_CXX_STANDARD 17)                                                                                    
set(CMAKE_CXX_STANDARD_REQUIRED ON)                                                                           
                                                                                                              
add_compile_options(-Wall -Werror)                                                                            
                                                                                                              
add_subdirectory (src)                                                                                
