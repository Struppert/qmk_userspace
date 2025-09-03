# QMK Userspace – neo

Dieses Repository enthält meinen **externen QMK-Userspace**.  
Es wird per `qmk config user.overlay_dir` als Overlay eingebunden und ersetzt so eigene Keymaps im QMK-Hauptrepo.

## Struktur
```
qmk_userspace/
├─ qmk.json                      # Userspace-Metadaten
├─ users/
│  └─ neo/                       # zentrale Logik (Makros, Unicode, etc.)
├─ keyboards/
│  └─ keychron/
│     ├─ q4/iso/keymaps/neo/     # Board-spezifische Keymaps
│     └─ v4/iso/keymaps/neo/     # weitere Board-Keymaps
└─ .gitignore
```

## Verwendung

### 1. Overlay aktivieren
```bash
qmk config user.overlay_dir="$(realpath ~/qmk_userspace)"
qmk doctor
# Erwartung: "Userspace enabled: True"
```

### 2. Keymaps bauen
Einzelnes Board:
```bash
qmk compile -kb keychron/q4/iso -km neo
qmk compile -kb keychron/v4/iso -km neo
```

Alle eingetragenen Targets in `qmk.json`:
```bash
qmk userspace-compile
```

### 3. Targets verwalten
Neues Target hinzufügen:
```bash
qmk userspace-add -kb <keyboard> -km neo
```

---

## Hinweise
- **Nur eigene Dateien** liegen hier; das QMK-Hauptrepo (`qmk_firmware`) bleibt separat.
- Änderungen an Layern, Makros etc. kommen in `users/neo/`.
- Board-spezifische Besonderheiten liegen in den jeweiligen `keymap.c` unter `keyboards/...`.
