file(REMOVE_RECURSE
  "creole.pdb"
  "creole"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/creole_convert.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
