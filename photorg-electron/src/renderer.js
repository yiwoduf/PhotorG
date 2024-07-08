document
  .getElementById("selectFolderBtn")
  .addEventListener("click", async () => {
    const path = await window.electronAPI.selectFolder();
    if (path) {
      document.getElementById("sourcePath").value = path;
    }
  });

document.getElementById("organizeBtn").addEventListener("click", async () => {
  const sourcePath = document.getElementById("sourcePath").value;
  const yearly = document.getElementById("yearly").checked;
  const monthly = document.getElementById("monthly").checked;
  const daily = document.getElementById("daily").checked;

  const result = await window.electronAPI.organizePhotos(
    sourcePath,
    yearly,
    monthly,
    daily
  );
  document.getElementById("result").textContent = result
    ? "Photos organized successfully!"
    : "Failed to organize photos.";
});

document.getElementById("viewTreeBtn").addEventListener("click", async () => {
  const sourcePath = document.getElementById("sourcePath").value;
  const tree = await window.electronAPI.getDirectoryTree(sourcePath);
  document.getElementById("result").textContent = tree.join("\n");
});
