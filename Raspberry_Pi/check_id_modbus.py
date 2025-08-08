#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Modbus RS485 Scanner
Quét và phát hiện các địa chỉ modbus slave qua RS485
"""

import time
import threading
from typing import List, Dict, Optional
from pymodbus.client import ModbusSerialClient
from pymodbus.exceptions import ModbusException, ConnectionException
import logging

# Cấu hình logging
logging.basicConfig(
    level=logging.INFO, format="%(asctime)s - %(levelname)s - %(message)s"
)
logger = logging.getLogger(__name__)


class ModbusRS485Scanner:
    """Lớp quét địa chỉ modbus qua RS485"""

    def __init__(
        self, port: str = "/dev/ttyUSB0", baudrate: int = 9600, timeout: float = 1.0
    ):
        """
        Khởi tạo scanner

        Args:
            port: Cổng serial (mặc định /dev/ttyUSB0)
            baudrate: Tốc độ baud (mặc định 9600)
            timeout: Thời gian timeout cho mỗi kết nối (giây)
        """
        self.port = port
        self.baudrate = baudrate
        self.timeout = timeout
        self.found_slaves = []
        self.scan_lock = threading.Lock()

    def scan_single_address(self, slave_id: int) -> Optional[Dict]:
        """
        Quét một địa chỉ slave cụ thể

        Args:
            slave_id: ID của slave cần quét

        Returns:
            Dict chứa thông tin slave nếu tìm thấy, None nếu không
        """
        client = None
        try:
            # Tạo kết nối modbus RS485
            client = ModbusSerialClient(
                method="rtu",
                port=self.port,
                baudrate=self.baudrate,
                timeout=self.timeout,
                parity="N",
                stopbits=1,
                bytesize=8,
            )

            if not client.connect():
                logger.debug(f"Không thể kết nối đến {self.port}")
                return None

            # Thử đọc holding registers để kiểm tra slave
            result = client.read_holding_registers(address=0, count=1, slave=slave_id)

            if result.isError():
                # Thử đọc input registers
                result = client.read_input_registers(address=0, count=1, slave=slave_id)

            if not result.isError():
                slave_info = {
                    "slave_id": slave_id,
                    "port": self.port,
                    "baudrate": self.baudrate,
                    "status": "active",
                    "timestamp": time.time(),
                }

                # Thử đọc thêm thông tin về slave
                try:
                    # Đọc device identification
                    device_info = client.read_device_information(slave=slave_id)
                    if not device_info.isError():
                        slave_info["device_info"] = device_info.registers
                except:
                    pass

                logger.info(f"Tìm thấy RS485 slave ID: {slave_id} trên {self.port}")
                return slave_info

        except (ModbusException, ConnectionException) as e:
            logger.debug(f"Lỗi Modbus khi quét slave {slave_id}: {str(e)}")
        except Exception as e:
            logger.error(f"Lỗi không xác định khi quét slave {slave_id}: {str(e)}")
        finally:
            if client:
                try:
                    client.close()
                except:
                    pass

        return None

    def scan_range(
        self, start_id: int = 1, end_id: int = 247, max_threads: int = 5
    ) -> List[Dict]:
        """
        Quét một dải địa chỉ slave

        Args:
            start_id: ID slave bắt đầu (mặc định 1)
            end_id: ID slave kết thúc (mặc định 247 - giới hạn modbus)
            max_threads: Số luồng tối đa để quét song song (giảm cho RS485)

        Returns:
            List các slave được tìm thấy
        """
        logger.info(f"Bắt đầu quét RS485 địa chỉ slave từ {start_id} đến {end_id}")

        self.found_slaves = []

        # Với RS485, quét tuần tự để tránh xung đột
        for slave_id in range(start_id, end_id + 1):
            try:
                print(f"Đang quét slave ID: {slave_id}/{end_id}...", end="\r")
                result = self.scan_single_address(slave_id)
                if result:
                    self.found_slaves.append(result)
                    print(f"✓ Tìm thấy slave ID: {slave_id}")
                else:
                    print(f"✗ Không tìm thấy slave ID: {slave_id}")
            except KeyboardInterrupt:
                print(f"\n\n⚠️  Người dùng dừng quét tại slave ID: {slave_id}")
                print(
                    f"Đã quét được {slave_id - start_id + 1}/{end_id - start_id + 1} địa chỉ"
                )
                break
            except Exception as e:
                print(f"✗ Lỗi khi quét slave ID: {slave_id} - {str(e)}")
                logger.error(f"Lỗi khi quét slave {slave_id}: {str(e)}")
                continue  # Tiếp tục quét slave tiếp theo

        print(f"\nHoàn thành quét từ {start_id} đến {end_id}")
        logger.info(f"Hoàn thành quét RS485. Tìm thấy {len(self.found_slaves)} slave")
        return self.found_slaves

    def scan_common_addresses(self) -> List[Dict]:
        """
        Quét các địa chỉ phổ biến

        Returns:
            List các slave được tìm thấy
        """
        common_addresses = [1, 2, 3, 4, 5, 10, 20, 30, 40, 50, 100, 200]
        logger.info(f"Quét các địa chỉ RS485 phổ biến: {common_addresses}")

        self.found_slaves = []
        for slave_id in common_addresses:
            result = self.scan_single_address(slave_id)
            if result:
                self.found_slaves.append(result)

        return self.found_slaves

    def print_results(self):
        """In kết quả quét"""
        if not self.found_slaves:
            print("Không tìm thấy RS485 slave nào")
            return

        print(f"\n=== KẾT QUẢ QUÉT MODBUS RS485 ===")
        print(f"Tổng số RS485 slave tìm thấy: {len(self.found_slaves)}")
        print(f"Port: {self.port}")
        print(f"Baudrate: {self.baudrate}")
        print("-" * 50)

        for i, slave in enumerate(self.found_slaves, 1):
            print(f"{i}. RS485 Slave ID: {slave['slave_id']}")
            print(f"   Trạng thái: {slave['status']}")
            print(
                f"   Thời gian phát hiện: {time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(slave['timestamp']))}"
            )
            if "device_info" in slave:
                print(f"   Thông tin thiết bị: {slave['device_info']}")
            print()


def main():
    """Hàm chính - Chỉ quét RS485"""
    print("=== MODBUS RS485 SCANNER ===")
    print("Quét Modbus RS485 slaves")

    # Nhập thông tin kết nối RS485
    port = input("Nhập cổng serial (mặc định /dev/ttyUSB0): ").strip() or "/dev/ttyUSB0"
    baudrate = int(input("Nhập baudrate (mặc định 9600): ").strip() or "9600")

    # Hỏi về chế độ debug
    debug_mode = input("Bật chế độ debug? (y/n, mặc định n): ").strip().lower() == "y"
    if debug_mode:
        logging.getLogger().setLevel(logging.DEBUG)
        print("✓ Chế độ debug đã bật")

    scanner = ModbusRS485Scanner(port=port, baudrate=baudrate)

    print("\nChọn phương thức quét RS485:")
    print("1. Quét dải địa chỉ")
    print("2. Quét địa chỉ phổ biến")

    scan_choice = input("Chọn (1-2): ").strip()

    if scan_choice == "1":
        start_id = int(input("Nhập ID bắt đầu (mặc định 1): ").strip() or "1")
        end_id = int(input("Nhập ID kết thúc (mặc định 247): ").strip() or "247")
        print(f"\nBắt đầu quét RS485 từ {start_id} đến {end_id}...")
        print("Nhấn Ctrl+C để dừng quét\n")
        scanner.scan_range(start_id, end_id)
    else:
        print("\nBắt đầu quét các địa chỉ RS485 phổ biến...")
        scanner.scan_common_addresses()

    scanner.print_results()


if __name__ == "__main__":
    main()
