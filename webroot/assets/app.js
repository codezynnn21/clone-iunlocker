const presets = {
  adreno750: { renderer: 'Adreno (TM) 750', vendor: 'Qualcomm', version: 'OpenGL ES 3.2 V@0686.0', shading: 'OpenGL ES GLSL ES 3.20' },
  adreno740: { renderer: 'Adreno (TM) 740', vendor: 'Qualcomm', version: 'OpenGL ES 3.2 V@0676.0', shading: 'OpenGL ES GLSL ES 3.20' },
  adreno730: { renderer: 'Adreno (TM) 730', vendor: 'Qualcomm', version: 'OpenGL ES 3.2 V@0615.0', shading: 'OpenGL ES GLSL ES 3.20' },
  adreno725: { renderer: 'Adreno (TM) 725', vendor: 'Qualcomm', version: 'OpenGL ES 3.2 V@0615.0', shading: 'OpenGL ES GLSL ES 3.20' },
  adreno660: { renderer: 'Adreno (TM) 660', vendor: 'Qualcomm', version: 'OpenGL ES 3.2 V@0530.0', shading: 'OpenGL ES GLSL ES 3.20' },
  adreno650: { renderer: 'Adreno (TM) 650', vendor: 'Qualcomm', version: 'OpenGL ES 3.2 V@0500.0', shading: 'OpenGL ES GLSL ES 3.20' },
  adreno642: { renderer: 'Adreno (TM) 642L', vendor: 'Qualcomm', version: 'OpenGL ES 3.2 V@0500.0', shading: 'OpenGL ES GLSL ES 3.20' },
  adreno640: { renderer: 'Adreno (TM) 640', vendor: 'Qualcomm', version: 'OpenGL ES 3.2 V@0500.0', shading: 'OpenGL ES GLSL ES 3.20' },
  adreno630: { renderer: 'Adreno (TM) 630', vendor: 'Qualcomm', version: 'OpenGL ES 3.2 V@0490.0', shading: 'OpenGL ES GLSL ES 3.20' },
  adreno620: { renderer: 'Adreno (TM) 620', vendor: 'Qualcomm', version: 'OpenGL ES 3.2 V@0500.0', shading: 'OpenGL ES GLSL ES 3.20' },
  adreno619: { renderer: 'Adreno (TM) 619', vendor: 'Qualcomm', version: 'OpenGL ES 3.2 V@0500.0', shading: 'OpenGL ES GLSL ES 3.20' },
  adreno618: { renderer: 'Adreno (TM) 618', vendor: 'Qualcomm', version: 'OpenGL ES 3.2 V@0500.0', shading: 'OpenGL ES GLSL ES 3.20' },
  adreno616: { renderer: 'Adreno (TM) 616', vendor: 'Qualcomm', version: 'OpenGL ES 3.2 V@0490.0', shading: 'OpenGL ES GLSL ES 3.20' },
  adreno612: { renderer: 'Adreno (TM) 612', vendor: 'Qualcomm', version: 'OpenGL ES 3.2 V@0490.0', shading: 'OpenGL ES GLSL ES 3.20' },
  adreno610: { renderer: 'Adreno (TM) 610', vendor: 'Qualcomm', version: 'OpenGL ES 3.2 V@0490.0', shading: 'OpenGL ES GLSL ES 3.20' },
  maliG720: { renderer: 'Mali-G720', vendor: 'ARM', version: 'OpenGL ES 3.2 v1.r44p1', shading: 'OpenGL ES GLSL ES 3.20' },
  maliG715: { renderer: 'Mali-G715', vendor: 'ARM', version: 'OpenGL ES 3.2 v1.r40p0', shading: 'OpenGL ES GLSL ES 3.20' },
  maliG710: { renderer: 'Mali-G710', vendor: 'ARM', version: 'OpenGL ES 3.2 v1.r38p1', shading: 'OpenGL ES GLSL ES 3.20' },
  maliG78:  { renderer: 'Mali-G78', vendor: 'ARM', version: 'OpenGL ES 3.2 v1.r32p1', shading: 'OpenGL ES GLSL ES 3.20' },
  maliG77:  { renderer: 'Mali-G77', vendor: 'ARM', version: 'OpenGL ES 3.2 v1.r28p0', shading: 'OpenGL ES GLSL ES 3.20' },
  maliG76:  { renderer: 'Mali-G76', vendor: 'ARM', version: 'OpenGL ES 3.2 v1.r16p0', shading: 'OpenGL ES GLSL ES 3.20' },
  maliG72:  { renderer: 'Mali-G72', vendor: 'ARM', version: 'OpenGL ES 3.2 v1.r12p0', shading: 'OpenGL ES GLSL ES 3.20' },
  maliG57:  { renderer: 'Mali-G57', vendor: 'ARM', version: 'OpenGL ES 3.2 v1.r32p1', shading: 'OpenGL ES GLSL ES 3.20' },
  maliG52:  { renderer: 'Mali-G52', vendor: 'ARM', version: 'OpenGL ES 3.2 v1.r25p1', shading: 'OpenGL ES GLSL ES 3.20' },
  immortalis: { renderer: 'Immortalis-G720', vendor: 'ARM', version: 'OpenGL ES 3.2 v1.r44p1', shading: 'OpenGL ES GLSL ES 3.20' },
  xclipse940: { renderer: 'Samsung Xclipse 940', vendor: 'Samsung', version: 'OpenGL ES 3.2', shading: 'OpenGL ES GLSL ES 3.20' },
  xclipse920: { renderer: 'Samsung Xclipse 920', vendor: 'Samsung', version: 'OpenGL ES 3.2', shading: 'OpenGL ES GLSL ES 3.20' },
  powervr: { renderer: 'PowerVR Rogue GE8320', vendor: 'Imagination', version: 'OpenGL ES 3.2', shading: 'OpenGL ES GLSL ES 3.20' },
  maleoon: { renderer: 'Maleoon 910', vendor: 'Huawei', version: 'OpenGL ES 3.2', shading: 'OpenGL ES GLSL ES 3.20' }
};

const $ = (id) => document.getElementById(id);

function load() {
  try {
    const s = JSON.parse(localStorage.getItem('iunlocker_gpu') || '{}');
    if (s.renderer) $('renderer').value = s.renderer;
    if (s.vendor) $('vendor').value = s.vendor;
    if (s.version) $('version').value = s.version;
    if (s.shading) $('shading').value = s.shading;
    $('props').checked = s.props !== false;
    $('paths').checked = s.paths !== false;
    if (s.ram) $('ram').value = s.ram;
  } catch (_) {}
}

function collect() {
  return {
    renderer: $('renderer').value.trim(),
    vendor: $('vendor').value.trim(),
    version: $('version').value.trim(),
    shading: $('shading').value.trim(),
    props: $('props').checked,
    paths: $('paths').checked,
    ram: parseInt($('ram').value, 10) || 0,
    updatedAt: new Date().toISOString()
  };
}

function toast(msg) {
  const t = $('toast');
  t.textContent = msg;
  t.classList.add('show');
  setTimeout(() => t.classList.remove('show'), 1500);
}

function setStatus(msg) { $('status').textContent = msg; }

async function writeConfig(cfg) {
  localStorage.setItem('iunlocker_gpu', JSON.stringify(cfg));
  if (window.ksu?.exec) {
    const json = JSON.stringify(cfg, null, 2);
    await window.ksu.exec(`mkdir -p /data/adb/iunlocker_codm && cat > /data/adb/iunlocker_codm/gpu.json << 'EOF'\n${json}\nEOF`);
  }
}

document.querySelectorAll('[data-ram]').forEach(btn => {
  btn.onclick = () => { $('ram').value = btn.dataset.ram; };
});

document.querySelectorAll('.chip[data-preset]').forEach(btn => {
  btn.onclick = () => {
    const p = presets[btn.dataset.preset];
    if (!p) return;
    $('renderer').value = p.renderer;
    $('vendor').value = p.vendor;
    $('version').value = p.version;
    $('shading').value = p.shading;
  };
});

$('saveBtn').onclick = async () => {
  setStatus('saving…');
  await writeConfig(collect());
  setStatus('saved');
  toast('Saved');
};

$('applyBtn').onclick = async () => {
  setStatus('applying…');
  await writeConfig(collect());
  setStatus('applied — reboot or relaunch game');
  toast('Applied');
};

load();
