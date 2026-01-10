# 提取并上传PDB文件到符号服务器

# 获取目标文件的调试信息
execute_process(
    COMMAND dumpbin /headers "$<TARGET_FILE:${PROJECT_NAME}>" /out:debug_info.txt
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# 解析调试信息获取GUID和Age
file(READ ${CMAKE_BINARY_DIR}/debug_info.txt DUMPBIN_OUTPUT)
string(REGEX MATCH "Format: PDB 7.00, {([0-9A-F-]+)}, ([0-9]+)" MATCH_RESULT ${DUMPBIN_OUTPUT})

if(MATCH_RESULT)
    set(PDB_GUID ${CMAKE_MATCH_1})
    set(PDB_AGE ${CMAKE_MATCH_2})
    
    # 构建PDB文件路径
    set(PDB_FILE "$<TARGET_PDB_FILE:${PROJECT_NAME}>")
    
    if(EXISTS ${PDB_FILE})
        # 计算PDB文件的哈希
        file(SHA256 ${PDB_FILE} PDB_HASH)
        
        # 构建上传JSON
        set(UPLOAD_JSON "{
            \"app_name\": \"${PROJECT_NAME}\",
            \"app_version\": \"${APP_VERSION_STRING}\",
            \"build_id\": \"${BUILD_ID}\",
            \"commit_hash\": \"${GIT_COMMIT_HASH}\",
            \"pdb_guid\": \"${PDB_GUID}\",
            \"pdb_age\": ${PDB_AGE},
            \"pdb_hash\": \"${PDB_HASH}\",
            \"build_time\": \"${BUILD_DATE} ${BUILD_TIME}\",
            \"platform\": \"windows\",
            \"architecture\": \"$<IF:$<CONFIG:Debug>,Debug,Release>\"
        }")
        
        # 上传到符号服务器
        find_program(CURL_EXECUTABLE curl)
        if(CURL_EXECUTABLE)
            # 创建临时文件
            file(WRITE ${CMAKE_BINARY_DIR}/symbol_info.json ${UPLOAD_JSON})
            
            # 上传PDB文件
            execute_process(
                COMMAND ${CURL_EXECUTABLE} -X POST 
                    -F "metadata=@${CMAKE_BINARY_DIR}/symbol_info.json"
                    -F "pdb_file=@${PDB_FILE}"
                    http://127.0.0.1:3410/upload/minidump
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            )
            
            message(STATUS "PDB文件已上传到符号服务器: ${PDB_GUID}-${PDB_AGE}")
        endif()
    endif()
endif()