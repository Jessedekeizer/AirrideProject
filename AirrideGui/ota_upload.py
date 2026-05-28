Import("env")


def _prompt_wifi(source, target, env):
    cfg = env.GetProjectConfig()
    ssid = cfg.get("ota_secrets", "ssid")
    password = cfg.get("ota_secrets", "pass")
    ip = cfg.get("ota_secrets", "ip")

    print()
    print("=== Airride OTA Upload ===")
    print("1. On device: open OTA screen and press YES")
    print(f"2. Connect PC to WiFi  ->  SSID: {ssid}  |  Pass: {password}")
    print(f"   Target IP: {ip}")
    input("Press Enter when connected...\n")


env.AddPreAction("upload", _prompt_wifi)
