const { app, BrowserWindow, ipcMain } = require("electron");
const path = require("path");
const photorg = require(path.join(__dirname, "../build/Release/photorg.node"));

let mainWindow;

function createWindow() {
  mainWindow = new BrowserWindow({
    width: 800,
    height: 600,
    webPreferences: {
      nodeIntegration: false,
      contextIsolation: true,
      preload: path.join(__dirname, "preload.js"),
    },
  });

  mainWindow.loadFile("src/index.html");

  // Send the current working directory path to the renderer process
  mainWindow.webContents.on("did-finish-load", () => {
    const currentPath = process.cwd();
    const photosPath = path.join(currentPath, "Photos");
    mainWindow.webContents.send("app-path", photosPath);
  });

  // Listen for window close event
  mainWindow.on("closed", () => {
    mainWindow = null;
    app.quit();
  });
}

app.on("ready", createWindow);

// Quit when all windows are closed
app.on("window-all-closed", () => {
  if (process.platform !== "darwin") {
    app.quit();
  }
});

app.on("activate", () => {
  if (mainWindow === null) {
    createWindow();
  }
});

ipcMain.handle(
  "organize-photos",
  async (event, sourcePath, yearly, monthly, daily) => {
    return photorg.organizePhotos(sourcePath, yearly, monthly, daily);
  }
);

ipcMain.handle("get-directory-tree", async (event, path) => {
  return photorg.getDirectoryTree(path);
});
