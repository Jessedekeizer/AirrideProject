Import("env")
import urllib.request
import base64
import os


def _ota_upload(target, source, env):
    cfg = env.GetProjectConfig()
    ssid = cfg.get("ota_secrets", "ssid")
    ap_pass = cfg.get("ota_secrets", "pass")

    try:
        ip = cfg.get("ota_secrets", "ip")
    except Exception:
        ip = "192.168.4.1"

    try:
        password = cfg.get("ota_secrets", "password")
    except Exception:
        password = None

    print()
    print("=== Airride OTA Upload ===")
    print(f"Connect PC to WiFi  ->  SSID: {ssid}  |  Pass: {ap_pass}")
    print(f"Target IP: {ip}")
    input("Press Enter when connected...\n")

    if not password:
        return

    firmware = env.subst("$BUILD_DIR/${PROGNAME}.bin")
    if not os.path.exists(firmware):
        print(f"Firmware not found: {firmware}")
        env.Exit(1)
        return

    with open(firmware, "rb") as f:
        data = f.read()

    auth = base64.b64encode(f"arduino:{password}".encode()).decode()
    req = urllib.request.Request(
        f"http://{ip}:65280/sketch",
        data=data,
        method="POST",
        headers={
            "Authorization": f"Basic {auth}",
            "Content-Length": str(len(data)),
        },
    )
    print(f"Uploading {len(data)} bytes to http://{ip}:65280/sketch ...")
    try:
        urllib.request.urlopen(req, timeout=30)
        print("Upload complete!")
    except urllib.error.HTTPError as e:
        print(f"Upload failed: HTTP {e.code} {e.reason}")
        env.Exit(1)
    except Exception as e:
        print(f"Upload failed: {e}")
        env.Exit(1)


try:
    upload_protocol = env.GetProjectOption("upload_protocol")
except Exception:
    upload_protocol = ""

if upload_protocol == "custom":
    env.Replace(UPLOADCMD=_ota_upload)
else:
    env.AddPreAction("upload", _ota_upload)
