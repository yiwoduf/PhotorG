const { contextBridge, ipcRenderer } = require("electron");

contextBridge.exposeInMainWorld("electronAPI", {
  organizePhotos: (sourcePath, yearly, monthly, daily) =>
    ipcRenderer.invoke("organize-photos", sourcePath, yearly, monthly, daily),
  getDirectoryTree: (path) => ipcRenderer.invoke("get-directory-tree", path),
  selectFolder: () => ipcRenderer.invoke("select-folder"),
});
