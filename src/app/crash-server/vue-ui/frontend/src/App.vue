<template>
  <div id="app">
    <div class="container">
      <header>
        <h1>崩溃分析管理系统</h1>
      </header>

      <main>
        <section class="search-section">
          <h2>查询崩溃记录</h2>
          <div class="form-row">
            <div class="form-group">
              <label for="apiKey">API密钥:</label>
              <input 
                type="password" 
                id="apiKey" 
                v-model="config.apiKey" 
                placeholder="请输入API密钥" 
              />
            </div>
            
            <div class="form-group">
              <label for="serverUrl">服务器地址:</label>
              <input 
                type="text" 
                id="serverUrl" 
                v-model="config.serverUrl" 
                placeholder="服务器地址" 
              />
            </div>
          </div>
          
          <div class="form-row">
            <div class="form-group">
              <label for="searchVersion">按版本号查询 (可选):</label>
              <input 
                type="text" 
                id="searchVersion" 
                v-model="searchVersion" 
                placeholder="例如: 1.0.0"
              />
            </div>
            
            <div class="form-group">
              <label for="searchGuid">按GUID查询 (可选):</label>
              <input 
                type="text" 
                id="searchGuid" 
                v-model="searchGuid" 
                placeholder="例如: 12345678-1234-1234-1234-1234567890AB"
              />
            </div>
          </div>
          
          <div class="button-group">
            <button @click="searchMinidumps(1)" :disabled="loading" class="btn-primary">
              {{ loading ? '查询中...' : '查询Minidump记录' }}
            </button>
            <button @click="clearResults" :disabled="loading" class="btn-secondary">
              清空结果
            </button>
          </div>
        </section>
        
        <section class="search-section">
          <h2>通过Minidump ID查询关联文件</h2>
          <div class="form-group">
            <label for="minidumpId">Minidump ID:</label>
            <input 
              type="text" 
              id="minidumpId" 
              v-model="minidumpId" 
              placeholder="输入Minidump ID"
            />
          </div>
          <button @click="findAssociatedFiles" :disabled="loading" class="btn-primary">
            {{ loading ? '查询中...' : '查找关联的PDB和EXE' }}
          </button>
        </section>
        
        <div v-if="message.text" :class="['message', message.type]">
          {{ message.text }}
        </div>
        
        <section class="results-section">
          <div class="results-header">
            <h2>查询结果</h2>
            <div class="sort-controls" v-if="minidumps.length > 0">
              <span>排序:</span>
              <select @change="sortByField" v-model="currentSortField">
                <option value="timestamp">时间戳</option>
                <option value="exe_version">EXE版本</option>
                <option value="exe_guid">EXE GUID</option>
                <option value="pid">进程ID</option>
                <option value="tid">线程ID</option>
                <option value="exception_code">异常代码</option>
              </select>
              <select @change="sortOrderChanged" v-model="currentSortOrder">
                <option value="asc">升序</option>
                <option value="desc">降序</option>
              </select>
            </div>
          </div>
          
          <div v-if="minidumps.length === 0" class="no-results">
            暂无查询结果
          </div>
          <div v-else>
            <table class="results-table">
              <thead>
                <tr>
                  <th @click="sortBy('id')" class="sortable">
                    ID <span v-if="currentSortField === 'id'">{{ currentSortOrder === 'asc' ? '↑' : '↓' }}</span>
                  </th>
                  <th @click="sortBy('timestamp')" class="sortable">
                    时间戳 <span v-if="currentSortField === 'timestamp'">{{ currentSortOrder === 'asc' ? '↑' : '↓' }}</span>
                  </th>
                  <th @click="sortBy('exe_version')" class="sortable">
                    EXE版本 <span v-if="currentSortField === 'exe_version'">{{ currentSortOrder === 'asc' ? '↑' : '↓' }}</span>
                  </th>
                  <th @click="sortBy('exe_guid')" class="sortable">
                    EXE GUID <span v-if="currentSortField === 'exe_guid'">{{ currentSortOrder === 'asc' ? '↑' : '↓' }}</span>
                  </th>
                  <th @click="sortBy('exe_age')" class="sortable">
                    EXE Age <span v-if="currentSortField === 'exe_age'">{{ currentSortOrder === 'asc' ? '↑' : '↓' }}</span>
                  </th>
                  <th @click="sortBy('pid')" class="sortable">
                    进程ID <span v-if="currentSortField === 'pid'">{{ currentSortOrder === 'asc' ? '↑' : '↓' }}</span>
                  </th>
                  <th @click="sortBy('tid')" class="sortable">
                    线程ID <span v-if="currentSortField === 'tid'">{{ currentSortOrder === 'asc' ? '↑' : '↓' }}</span>
                  </th>
                  <th @click="sortBy('exception_code')" class="sortable">
                    异常代码 <span v-if="currentSortField === 'exception_code'">{{ currentSortOrder === 'asc' ? '↑' : '↓' }}</span>
                  </th>
                  <th>操作</th>
                </tr>
              </thead>
              <tbody>
                <tr v-for="record in minidumps" :key="record.id">
                  <td>{{ record.id.substring(0, 8) }}...</td>
                  <td>{{ record.timestamp }}</td>
                  <td>{{ record.exe_version }}</td>
                  <td>{{ record.exe_guid.substring(0, 8) }}...</td>
                  <td>{{ record.exe_age }}</td>
                  <td>{{ record.pid }}</td>
                  <td>{{ record.tid }}</td>
                  <td>{{ record.exception_code }}</td>
                  <td>
                    <button @click="downloadFile(record.id, 'minidump')" class="btn-download">下载</button>
                    <button @click="findAssociatedFilesById(record.id)" class="btn-associated">关联文件</button>
                  </td>
                </tr>
              </tbody>
            </table>
          </div>
        </section>
        
        <section v-if="associatedFiles" class="associated-files">
          <h2>关联文件</h2>
          <div class="file-info" v-if="associatedFiles.pdb">
            <h3>PDB 文件</h3>
            <p><strong>ID:</strong> {{ associatedFiles.pdb.id }}</p>
            <p><strong>文件名:</strong> {{ associatedFiles.pdb.pdb_filename }}</p>
            <p><strong>版本:</strong> {{ associatedFiles.pdb.exe_version }}</p>
            <p><strong>GUID:</strong> {{ associatedFiles.pdb.exe_guid }}</p>
            <p><strong>Age:</strong> {{ associatedFiles.pdb.exe_age }}</p>
            <button @click="downloadFile(associatedFiles.pdb.id, 'pdb')" class="btn-download">下载PDB</button>
          </div>
          <div class="file-info" v-else>
            <h3>PDB 文件</h3>
            <p>未找到关联的PDB文件</p>
          </div>
          
          <div class="file-info" v-if="associatedFiles.exe">
            <h3>EXE 文件</h3>
            <p><strong>ID:</strong> {{ associatedFiles.exe.id }}</p>
            <p><strong>文件名:</strong> {{ associatedFiles.exe.exe_filename }}</p>
            <p><strong>版本:</strong> {{ associatedFiles.exe.exe_version }}</p>
            <p><strong>GUID:</strong> {{ associatedFiles.exe.exe_guid }}</p>
            <button @click="downloadFile(associatedFiles.exe.id, 'exe')" class="btn-download">下载EXE</button>
          </div>
          <div class="file-info" v-else>
            <h3>EXE 文件</h3>
            <p>未找到关联的EXE文件</p>
          </div>
        </section>
        
        <!-- 分页控件 -->
        <section v-if="totalPages > 1" class="pagination-section">
          <div class="pagination">
            <button 
              @click="(e) => goToPage(currentPage - 1)" 
              :disabled="currentPage <= 1"
              class="btn-pagination">
              上一页
            </button>
            
            <span class="page-info">
              第 {{ currentPage }} 页，共 {{ totalPages }} 页 ({{ totalRecords }} 条记录)
            </span>
            
            <button 
              @click="(e) => goToPage(currentPage + 1)" 
              :disabled="currentPage >= totalPages"
              class="btn-pagination">
              下一页
            </button>
          </div>
        </section>
      </main>
    </div>
  </div>
</template>

<script lang="ts">
import { defineComponent } from 'vue';
import axios from 'axios';

// 定义类型
interface Minidump {
  id: string;
  timestamp: string;
  exe_version: string;
  exe_guid: string;
  exe_age: number;
  pid: number;
  tid: number;
  exception_code: string;
  exception_address: string | null;
  file_path: string;
  created_at: string;
}

interface Pdb {
  id: string;
  exe_version: string;
  exe_guid: string;
  exe_age: number;
  pdb_filename: string;
  file_path: string;
  uploaded_at: string;
}

interface Exe {
  id: string;
  exe_version: string;
  exe_guid: string;
  exe_filename: string;
  file_path: string;
  uploaded_at: string;
}

interface AssociatedFiles {
  pdb: Pdb | null;
  exe: Exe | null;
}

interface Config {
  apiKey: string;
  serverUrl: string;
}

export default defineComponent({
  name: 'App',
  data() {
    return {
      config: {
        apiKey: 'test-api-key-123',
        serverUrl: 'http://localhost:3410',
      } as Config,
      searchVersion: '',
      searchGuid: '',
      minidumpId: '',
      minidumps: [] as Minidump[],
      associatedFiles: null as AssociatedFiles | null,
      loading: false,
      message: {
        type: 'info',
        text: ''
      },
      currentSortField: 'timestamp',
      currentSortOrder: 'desc' as 'asc' | 'desc',  // 确保类型正确
      currentPage: 1,  // 当前页码
      totalPages: 1,   // 总页数
      totalRecords: 0  // 总记录数
    }
  },
  mounted() {
    // 页面加载时设置默认服务器地址
    this.config.serverUrl = window.location.protocol + '//' + window.location.hostname + ':3410';
  },
  methods: {
    async searchMinidumps(page: number = 1) {
      this.loading = true;
      try {
        const params: any = {};
        if (this.searchVersion) params.version = this.searchVersion;
        if (this.searchGuid) params.guid = this.searchGuid;
        params.page = page;  // 添加页码参数
        params.page_size = 20;  // 每页显示数量

        const response = await axios.get(`${this.config.serverUrl}/list/minidumps`, {
          headers: { 'X-API-Key': this.config.apiKey },
          params
        });

        if (response.data.success) {
          this.minidumps = response.data.data.minidumps || [];
          this.totalRecords = response.data.data.total || 0;
          this.currentPage = response.data.data.page || 1;
          this.totalPages = response.data.data.pages || 1;
          
          // 默认按时间倒序排列
          this.sortMinidumps(this.currentSortField, this.currentSortOrder);
          this.showMessage(`找到 ${this.totalRecords} 条记录，当前显示第 ${this.currentPage} 页，共 ${this.totalPages} 页`, 'success');
        } else {
          this.showMessage(response.data.error || '查询失败', 'error');
        }
      } catch (error: any) {
        this.showMessage(`查询失败: ${error.message}`, 'error');
      } finally {
        this.loading = false;
      }
    },

    async findAssociatedFiles() {
      if (!this.minidumpId) {
        this.showMessage('请输入 Minidump ID', 'error');
        return;
      }

      this.loading = true;
      try {
        // 获取Minidump详情
        const minidump = this.minidumps.find(m => m.id === this.minidumpId);
        
        if (!minidump) {
          // 如果不在列表中，尝试从服务器获取
          const listResponse = await axios.get(`${this.config.serverUrl}/list/minidumps`, {
            headers: { 'X-API-Key': this.config.apiKey }
          });

          if (listResponse.data.success) {
            const found = listResponse.data.data.minidumps.find((m: Minidump) => m.id === this.minidumpId);
            if (!found) {
              this.showMessage('找不到指定 ID 的 Minidump 记录', 'error');
              return;
            }
            this.findAssociatedFilesByRecord(found.exe_version, found.exe_guid, found.exe_age);
          }
        } else {
          this.findAssociatedFilesByRecord(minidump.exe_version, minidump.exe_guid, minidump.exe_age);
        }
      } catch (error: any) {
        this.showMessage(`查找关联文件失败: ${error.message}`, 'error');
      } finally {
        this.loading = false;
      }
    },

    async findAssociatedFilesById(id: string) {
      this.minidumpId = id;
      this.findAssociatedFiles();
    },

    async findAssociatedFilesByRecord(exeVersion: string, exeGuid: string, exeAge: number) {
      this.loading = true;
      try {
        // 查找匹配的PDB
        let pdbData: Pdb | null = null;
        try {
          const pdbResponse = await axios.get(`${this.config.serverUrl}/find/matching-pdb`, {
            headers: { 'X-API-Key': this.config.apiKey },
            params: {
              exe_version: exeVersion,
              exe_guid: exeGuid,
              exe_age: exeAge
            }
          });
          
          if (pdbResponse.data.success && pdbResponse.data.data && pdbResponse.data.data.pdb) {
            pdbData = pdbResponse.data.data.pdb;
          }
        } catch (error) {
          console.error("获取PDB失败", error);
        }

        // 查找关联的EXE
        let exeData: Exe | null = null;
        try {
          const exeResponse = await axios.get(`${this.config.serverUrl}/find/exe-by-guid`, {
            headers: { 'X-API-Key': this.config.apiKey },
            params: {
              exe_guid: exeGuid
            }
          });
          
          if (exeResponse.data.success && exeResponse.data.data && exeResponse.data.data.exe) {
            exeData = exeResponse.data.data.exe;
          }
        } catch (error) {
          console.error("获取EXE失败", error);
        }

        this.associatedFiles = { pdb: pdbData, exe: exeData };
        this.showMessage('成功找到关联文件', 'success');
      } catch (error: any) {
        this.showMessage(`查找关联文件失败: ${error.message}`, 'error');
      } finally {
        this.loading = false;
      }
    },

    downloadFile(id: string, type: 'minidump' | 'pdb' | 'exe') {
      // 使用fetch API以支持添加API密钥到请求头
      fetch(`${this.config.serverUrl}/download/${type}/${id}`, {
        method: 'GET',
        headers: {
          'X-API-Key': this.config.apiKey
        }
      })
      .then(response => {
        if (!response.ok) {
          if(response.status === 404) {
            throw new Error(`File not found (404): ${type} with id ${id}`);
          } else {
            throw new Error(`HTTP error! status: ${response.status}`);
          }
        }
        return response.blob();
      })
      .then(blob => {
        // 创建下载链接
        const link = document.createElement('a');
        link.href = window.URL.createObjectURL(blob);
        link.download = `${type}_${id}.${type === 'exe' ? 'exe' : type === 'pdb' ? 'pdb' : 'dmp'}`;
        document.body.appendChild(link);
        link.click();
        document.body.removeChild(link);
        window.URL.revokeObjectURL(link.href);
      })
      .catch(error => {
        console.error("下载失败:", error);
        this.showMessage(`下载失败: ${error.message}`, 'error');
      });
    },

    showMessage(text: string, type: 'info' | 'success' | 'error') {
      this.message = { type, text };
      setTimeout(() => {
        this.message.text = '';
      }, 5000);
    },
    
    sortBy(field: string) {
      if (this.currentSortField === field) {
        // 如果当前已经是该字段排序，则切换顺序
        this.currentSortOrder = this.currentSortOrder === 'asc' ? 'desc' : 'asc';
      } else {
        // 否则切换到新字段并默认升序
        this.currentSortField = field;
        this.currentSortOrder = 'asc';
      }
      this.sortMinidumps(this.currentSortField, this.currentSortOrder);
    },
    
    sortByField() {
      this.sortMinidumps(this.currentSortField, this.currentSortOrder);
    },
    
    sortOrderChanged() {
      this.sortMinidumps(this.currentSortField, this.currentSortOrder);
    },
    
    sortMinidumps(field: string, order: 'asc' | 'desc') {
      this.minidumps.sort((a, b) => {
        let valueA: any = a[field as keyof Minidump];
        let valueB: any = b[field as keyof Minidump];
        
        // 如果是字符串类型，转换为小写进行比较
        if (typeof valueA === 'string') {
          valueA = valueA.toLowerCase();
          valueB = valueB.toLowerCase();
        }
        
        // 如果是数字类型，直接比较
        if (order === 'asc') {
          return valueA > valueB ? 1 : valueA < valueB ? -1 : 0;
        } else {
          return valueA < valueB ? 1 : valueA > valueB ? -1 : 0;
        }
      });
    },
    
    clearResults() {
      this.minidumps = [];
      this.associatedFiles = null;
      this.searchVersion = '';
      this.searchGuid = '';
      this.minidumpId = '';
      this.showMessage('已清空查询结果', 'info');
    },
    
    goToPage(page: number | Event) {
      // 如果传入的是事件对象，则忽略
      if (typeof page !== 'number') return;
      
      if (page >= 1 && page <= this.totalPages) {
        this.searchMinidumps(page);
      }
    }
  }
});
</script>

<style>
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  color: #2c3e50;
  margin: 0;
  background-color: #f0f2f5;
}

.container {
  max-width: 1400px;
  margin: 0 auto;
  padding: 20px;
  background-color: #fff;
  min-height: 100vh;
  box-shadow: 0 0 10px rgba(0,0,0,0.05);
}

header {
  text-align: center;
  margin-bottom: 30px;
  padding: 20px 0;
  background: linear-gradient(to right, #007bff, #0056b3);
  color: white;
  border-radius: 8px;
  box-shadow: 0 4px 6px rgba(0,0,0,0.1);
}

h1 {
  color: white;
  margin: 0;
  font-size: 2.2em;
  text-shadow: 1px 1px 3px rgba(0,0,0,0.3);
}

h2 {
  color: #444;
  margin-top: 0;
  padding-bottom: 10px;
  border-bottom: 2px solid #007bff;
  display: inline-block;
}

.search-section {
  margin-bottom: 30px;
  padding: 25px;
  background-color: #f8f9fa;
  border-radius: 10px;
  box-shadow: 0 2px 10px rgba(0,0,0,0.05);
}

.form-row {
  display: flex;
  gap: 20px;
  margin-bottom: 15px;
  flex-wrap: wrap;
}

.form-group {
  flex: 1;
  min-width: 250px;
}

label {
  display: block;
  margin-bottom: 5px;
  font-weight: bold;
  color: #555;
}

input[type="text"], input[type="password"] {
  width: 100%;
  padding: 10px;
  border: 1px solid #ddd;
  border-radius: 4px;
  font-size: 14px;
  transition: border-color 0.3s;
}

input[type="text"]:focus, input[type="password"]:focus {
  outline: none;
  border-color: #007bff;
  box-shadow: 0 0 5px rgba(0,123,255,0.3);
}

.button-group {
  display: flex;
  gap: 10px;
  margin-top: 15px;
}

.btn-primary {
  background-color: #007bff;
  color: white;
  border: none;
  padding: 12px 20px;
  border-radius: 4px;
  cursor: pointer;
  font-size: 16px;
  transition: background-color 0.3s;
}

.btn-primary:hover:not(:disabled) {
  background-color: #0056b3;
}

.btn-primary:disabled {
  background-color: #6c757d;
  cursor: not-allowed;
}

.btn-secondary {
  background-color: #6c757d;
  color: white;
  border: none;
  padding: 12px 20px;
  border-radius: 4px;
  cursor: pointer;
  font-size: 16px;
  transition: background-color 0.3s;
}

.btn-secondary:hover:not(:disabled) {
  background-color: #545b62;
}

.btn-download {
  background-color: #28a745;
  color: white;
  padding: 6px 12px;
  border: none;
  border-radius: 4px;
  cursor: pointer;
  font-size: 14px;
  margin-right: 5px;
  transition: background-color 0.3s;
}

.btn-download:hover {
  background-color: #218838;
}

.btn-associated {
  background-color: #ffc107;
  color: #212529;
  padding: 6px 12px;
  border: none;
  border-radius: 4px;
  cursor: pointer;
  font-size: 14px;
  transition: background-color 0.3s;
}

.btn-associated:hover {
  background-color: #e0a800;
}

.pagination-section {
  margin-top: 20px;
  text-align: center;
}

.pagination {
  display: flex;
  justify-content: center;
  align-items: center;
  gap: 15px;
  padding: 20px;
}

.btn-pagination {
  background-color: #007bff;
  color: white;
  border: none;
  padding: 10px 15px;
  border-radius: 4px;
  cursor: pointer;
  font-size: 14px;
  transition: background-color 0.3s;
}

.btn-pagination:hover:not(:disabled) {
  background-color: #0056b3;
}

.btn-pagination:disabled {
  background-color: #6c757d;
  cursor: not-allowed;
  opacity: 0.6;
}

.page-info {
  font-weight: bold;
  color: #495057;
}

.results-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 15px;
}

.sort-controls {
  display: flex;
  align-items: center;
  gap: 10px;
}

.sort-controls select {
  padding: 5px;
  border: 1px solid #ddd;
  border-radius: 4px;
}

.results-table {
  width: 100%;
  border-collapse: collapse;
  margin-top: 15px;
  box-shadow: 0 1px 3px rgba(0,0,0,0.1);
  border-radius: 8px;
  overflow: hidden;
}

.results-table th,
.results-table td {
  border: 1px solid #dee2e6;
  padding: 12px;
  text-align: left;
}

.results-table th {
  background-color: #007bff;
  color: white;
  font-weight: bold;
  text-align: center;
}

.results-table th.sortable {
  cursor: pointer;
  user-select: none;
  position: relative;
  transition: background-color 0.2s;
}

.results-table th.sortable:hover {
  background-color: #0069d9;
}

.results-table tr:nth-child(even) {
  background-color: #f8f9fa;
}

.results-table tr:hover {
  background-color: #e9ecef;
}

.no-results {
  text-align: center;
  padding: 40px;
  color: #666;
  font-size: 18px;
}

.message {
  padding: 15px;
  margin: 15px 0;
  border-radius: 4px;
  font-weight: 500;
}

.message.success {
  background-color: #d4edda;
  color: #155724;
  border: 1px solid #c3e6cb;
}

.message.error {
  background-color: #f8d7da;
  color: #721c24;
  border: 1px solid #f5c6cb;
}

.message.info {
  background-color: #d1ecf1;
  color: #0c5460;
  border: 1px solid #bee5eb;
}

.associated-files {
  margin-top: 30px;
  padding: 25px;
  background-color: #fff;
  border-radius: 10px;
  box-shadow: 0 2px 10px rgba(0,0,0,0.05);
  border: 1px solid #e9ecef;
}

.file-info {
  margin-bottom: 25px;
  padding: 15px;
  background-color: #f8f9fa;
  border-left: 4px solid #007bff;
  border-radius: 4px;
}

.file-info h3 {
  margin-top: 0;
  color: #007bff;
  border-bottom: 1px dashed #ccc;
  padding-bottom: 5px;
}

@media (max-width: 768px) {
  .form-row {
    flex-direction: column;
  }
  
  .results-header {
    flex-direction: column;
    align-items: flex-start;
    gap: 10px;
  }
  
  .container {
    padding: 10px;
  }
  
  .search-section {
    padding: 15px;
  }
}
</style>