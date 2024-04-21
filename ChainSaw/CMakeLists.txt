set(GRAPHVIZ_PREFIX "C:/Program Files/Graphviz")

set(libcgraph
        ${GRAPHVIZ_PREFIX}/lib/cdt.lib
        ${GRAPHVIZ_PREFIX}/lib/cgraph.lib
        ${GRAPHVIZ_PREFIX}/lib/cgraph++.lib
)
set(libgvc
        ${GRAPHVIZ_PREFIX}/lib/gvc.lib
        ${GRAPHVIZ_PREFIX}/lib/gvc++.lib
        ${GRAPHVIZ_PREFIX}/lib/cgraph.lib
        ${GRAPHVIZ_PREFIX}/lib/cgraph++.lib
        ${GRAPHVIZ_PREFIX}/lib/cdt.lib

        ${GRAPHVIZ_PREFIX}/lib/gvplugin_core.lib
        ${GRAPHVIZ_PREFIX}/lib/gvplugin_dot_layout.lib
        ${GRAPHVIZ_PREFIX}/lib/gvplugin_neato_layout.lib
        ${GRAPHVIZ_PREFIX}/lib/gvplugin_gdiplus.lib
        ${GRAPHVIZ_PREFIX}/lib/gvplugin_pango.lib
)

file(GLOB_RECURSE ChainSaw_src "src/**.c" "src/**.cpp" "include/**.h" "include/**.hpp")
add_executable(ChainSaw ${ChainSaw_src})
target_include_directories(ChainSaw PUBLIC "include" ${GRAPHVIZ_PREFIX}/include)
target_link_libraries(ChainSaw PUBLIC ${libcgraph} ${libgvc})
