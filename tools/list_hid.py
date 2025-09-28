# save as: list_hid.py
import hid

for d in hid.enumerate():
    print(f"PATH={d.get('path')}")
    print(f"  VID=0x{d.get('vendor_id', 0):04x}  PID=0x{d.get('product_id', 0):04x}")
    up = d.get("usage_page")
    print(
        f"  usage_page={hex(up) if up is not None else 'n/a'}  interface={d.get('interface_number')}"
    )
    print(
        f"  manufacturer={d.get('manufacturer_string')} product={d.get('product_string')}"
    )
    print("-" * 60)
