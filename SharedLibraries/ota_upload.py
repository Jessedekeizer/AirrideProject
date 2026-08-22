Import("env")
import urllib.request
import base64
import os
import platform
import subprocess
import tempfile
import time

WLAN_PROFILE_TEMPLATE = """<?xml version="1.0"?>
<WLANProfile xmlns="http://www.microsoft.com/networking/WLAN/profile/v1">
    <name>{ssid}</name>
    <SSIDConfig>
        <SSID>
            <name>{ssid}</name>
        </SSID>
    </SSIDConfig>
    <connectionType>ESS</connectionType>
    <connectionMode>manual</connectionMode>
    <MSM>
        <security>
            <authEncryption>
                <authentication>WPA2PSK</authentication>
                <encryption>AES</encryption>
                <useOneX>false</useOneX>
            </authEncryption>
            <sharedKey>
                <keyType>passPhrase</keyType>
                <protected>false</protected>
                <keyMaterial>{password}</keyMaterial>
            </sharedKey>
        </security>
    </MSM>
</WLANProfile>
"""


def _connect_wifi_windows(ssid, ap_pass, timeout=20):
    xml = WLAN_PROFILE_TEMPLATE.format(ssid=ssid, password=ap_pass)
    with tempfile.NamedTemporaryFile(mode="w", suffix=".xml", delete=False) as f:
        f.write(xml)
        profile_path = f.name

    try:
        subprocess.run(
            ["netsh", "wlan", "add", "profile", f"filename={profile_path}"],
            check=True, capture_output=True, text=True,
        )
        subprocess.run(
            ["netsh", "wlan", "connect", f"name={ssid}", f"ssid={ssid}"],
            check=True, capture_output=True, text=True,
        )

        deadline = time.time() + timeout
        while time.time() < deadline:
            result = subprocess.run(
                ["netsh", "wlan", "show", "interfaces"],
                capture_output=True, text=True,
            )
            out = result.stdout
            if ssid in out and "State" in out and "connected" in out.lower():
                return True
            time.sleep(1)
        return False
    except Exception as e:
        print(f"Auto-connect failed: {e}")
        return False
    finally:
        try:
            os.remove(profile_path)
        except OSError:
            pass


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

    connected = False
    if platform.system() == "Windows":
        print("Attempting auto-connect...")
        connected = _connect_wifi_windows(ssid, ap_pass)
        if connected:
            print(f"Connected to {ssid}.")
        else:
            print("Auto-connect failed, connect manually.")

    if not connected:
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
