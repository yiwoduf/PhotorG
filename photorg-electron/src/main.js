const { app, BrowserWindow, ipcMain, dialog } = require("electron");
const path = require("path");

let mainWindow;
let photorg;

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

  mainWindow.on("closed", () => {
    mainWindow = null;
    app.quit();
  });
}

app.on("ready", () => {
  createWindow();

  // Load the photorg module after the app is ready
  let photorgPath;
  if (app.isPackaged) {
    photorgPath = path.join(
      process.resourcesPath,
      "app.asar.unpacked",
      "build",
      "Release",
      "photorg.node"
    );
  } else {
    photorgPath = path.join(
      __dirname,
      "..",
      "build",
      "Release",
      "photorg.node"
    );
  }

  try {
    photorg = require(photorgPath);
  } catch (error) {
    console.error("Failed to load photorg module:", error);
  }
});

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
    if (!photorg) {
      throw new Error("photorg module not loaded");
    }
    return photorg.organizePhotos(sourcePath, yearly, monthly, daily);
  }
);

ipcMain.handle("get-directory-tree", async (event, path) => {
  if (!photorg) {
    throw new Error("photorg module not loaded");
  }
  return photorg.getDirectoryTree(path);
});

ipcMain.handle("select-folder", async () => {
  const result = await dialog.showOpenDialog(mainWindow, {
    properties: ["openDirectory"],
  });
  if (result.canceled) {
    return null;
  } else {
    return result.filePaths[0];
  }
});
