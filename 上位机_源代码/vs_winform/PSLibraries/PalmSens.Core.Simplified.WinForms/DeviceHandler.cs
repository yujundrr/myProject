using System;
using System.Collections.Generic;
using System.Data;
using System.Diagnostics.Contracts;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using PalmSens.Annotations;
using PalmSens.Comm;
using PalmSens.Devices;
using PalmSens.Windows.Devices;

namespace PalmSens.Core.Simplified.WinForms
{
    public class DeviceHandler
    {
        public DeviceHandler()
        {}

        public Device[] ConnectedDevices { get { return ScanDevices(); } }

        public async Task<Device[]> GetConnectedDevicesAsync() { return await ScanDevicesAsync(); }

        public bool EnableBluetooth = false;

        public bool EnableSerialPort = false;

        /// <summary>
        /// Scans for connected devices.扫描连接的设备。
        /// </summary>
        /// <returns>
        /// Returns an array of connected devices返回连接设备的数组
        /// </returns>
        /// <exception异常 cref="System.ArgumentException">An error occured while attempting to scan for connected devices试图扫描连接的设备时出错。.</exception>
        private Device[] ScanDevices()
        {
            Device[] devices = new Device[0];
            string errors = "";

            try //Attempt to find connected palmsens/emstat devices尝试查找连接的palmsens/emstat设备
            {
                List<DeviceList.DiscoverDevicesFunc> discFuncs = new List<DeviceList.DiscoverDevicesFunc>();
                //Add delegates to list for finding devices on specific communication protocols
                //添加委托到列表中，用于查找特定通信协议上的设备
                discFuncs.Add(USBCDCDevice.DiscoverDevices); //Default for PS4默认为PS4
                discFuncs.Add(FTDIDevice.DiscoverDevices); //Default for Emstat + PS3默认为Emstat + PS3
                if (EnableSerialPort)
                    discFuncs.Add(SerialPortDevice.DiscoverDevices); //Devices connected via serial port通过串口连接的设备
                if (EnableBluetooth)
                {
                    discFuncs.Add(BluetoothDevice.DiscoverDevices); //Bluetooth devices (PS4, PS3, Emstat Blue)
                    //discFuncs.Add(BLEDevice.DiscoverDevices); //BLEDevices requires adding a reference to the PalmSens.Core.Windows.BLE.dll
                }

                //Return a new array of connected devices found with the included delegate functions 
                devices = new DeviceList(discFuncs).GetAvailableDevices(out errors, EnableBluetooth);

            }
            catch (Exception)
            {
                throw new ArgumentException($"An error occured while attempting to scan for connected devices. {Environment.NewLine} {errors}");
            }
            return devices;
        }

        /// <summary>
        /// Scans for connected devices.扫描连接的设备。
        /// </summary>
        /// <returns>
        /// Returns an array of connected devices返回连接设备的数组
        /// </returns>
        /// <exception cref="System.ArgumentException">An error occured while attempting to scan for connected devices.</exception>
        private async Task<Device[]> ScanDevicesAsync()
        {
            List<Device> devices = new List<Device>();
            string errors = "";

            await new SynchronizationContextRemover();

            try //Attempt to find connected palmsens/emstat devices尝试查找连接的palmsens/emstat设备
            {
                List<Task<List<Device>>> discFuncs = new List<Task<List<Device>>>();
                //Add delegates to list for finding devices on specific communication protocols添加委托到列表中，用于查找特定通信协议上的设备
                discFuncs.Add(USBCDCDevice.DiscoverDevicesAsync()); //Default for PS4默认为PS4
                discFuncs.Add(FTDIDevice.DiscoverDevicesAsync()); //Default for Emstat + PS3默认为Emstat + PS3
                if (EnableSerialPort)
                    discFuncs.Add(SerialPortDevice.DiscoverDevicesAsync()); //Devices connected via serial port通过串口连接的设备
                if (EnableBluetooth)
                {
                    discFuncs.Add(BluetoothDevice.DiscoverDevicesAsync()); //Bluetooth devices (PS4, PS3, Emstat Blue)蓝牙设备(PS4, PS3, Emstat Blue)
                    //discFuncs.Add(BLEDevice.DiscoverDevicesAsync()); //BLEDevices requires adding a reference to the PalmSens.Core.Windows.BLE.dll
                }
                //Return a new array of connected devices found with the included delegate functions 返回包含委托函数的连接设备的新数组
                await Task.WhenAll(discFuncs);
                foreach (Task<List<Device>> task in discFuncs)
                    devices.AddRange(task.Result);
            }
            catch (Exception)
            {
                throw new ArgumentException($"An error occured while attempting to scan for connected devices.试图扫描连接的设备时出错。 {Environment.NewLine} {errors}");
            }
            return devices.ToArray();
        }

        /// <summary>
        /// Connects to the first device in the connected devices array and returns its CommManager.连接到已连接设备数组中的第一个设备，并返回其CommManager。
        /// </summary>
        /// <returns>
        /// The CommManager of the device it connected to or null它所连接的设备的CommManager或为空
        /// </returns>
        /// <exception cref="System.Exception">Could not find a device to connect to.</exception>
        internal CommManager Connect()
        {
            CommManager comm = null;
            Device[] devices = ScanDevices();
            if (devices.Length == 0)
                throw new Exception("Could not find a device to connect to.找不到要连接的设备。");
            else
                comm = Connect(devices[0]);
            return comm;
        }

        /// <summary>
        /// Connects to the first device in the connected devices array and returns its CommManager.
        /// </summary>
        /// <returns>
        /// The CommManager of the device it connected to or null
        /// </returns>
        /// <exception cref="System.Exception">Could not find a device to connect to.</exception>
        internal async Task<CommManager> ConnectAsync()
        {
            CommManager comm = null;
            Device[] devices = await ScanDevicesAsync();
            if (devices.Length == 0)
                throw new Exception("Could not find a device to connect to.找不到要连接的设备");
            else
                (comm, _, _) = await ConnectAsync(devices[0], -1, true);
            return comm;
        }

        /// <summary>
        /// Connects to the specified device and returns its CommManager.连接到指定的设备并返回其CommManager。
        /// </summary>
        /// <param name="device">The device.</param>
        /// <returns>
        /// The CommManager of the device or null
        /// </returns>
        /// <exception cref="System.ArgumentNullException">The specified device cannot be null.</exception>
        /// <exception cref="System.Exception">Could not connect to the specified device.</exception>
        internal CommManager Connect(Device device)
        {
            if (device == null)
                throw new ArgumentNullException("The specified device cannot be null.指定的设备不能为空");
            CommManager comm = null;

            try
            {
                device.Open(); //Open the device to allow a connection打开设备以允许连接
                comm = new CommManager(device); //Connect to the selected device连接到所选设备
            }
            catch (Exception ex)
            {
                device.Close();
                throw new Exception($"Could not connect to the specified device.无法连接到指定的设备 {ex.Message}");
            }

            return comm;
        }

        /// <summary>
        /// Connects to the specified device and returns its CommManager.连接到指定的设备并返回其CommManager。
        /// </summary>
        /// <param name="device">The device.</param>
        /// <returns>
        /// The CommManager of the device or null
        /// </returns>
        /// <exception cref="System.ArgumentNullException">The specified device cannot be null.</exception>
        /// <exception cref="System.Exception">Could not connect to the specified device.</exception>
        internal async Task<(CommManager Comm, int ChannelIndex, Exception Exception)> ConnectAsync(Device device, int channel = -1, bool throwExceptions = true)
        {
            if (device == null)
                throw new ArgumentNullException("The specified device cannot be null.");
            CommManager comm = null;
            Exception ex = null;

            await new SynchronizationContextRemover();

            try
            {
                await device.OpenAsync(); //Open the device to allow a connection
                comm = await CommManager.CommManagerAsync(device); //Connect to the selected device
                if (channel > -1)
                    comm.ChannelIndex = channel;
            }
            catch (Exception exception)
            {
                device.Close();
                ex = new Exception(channel > -1 ? $"Could not connect to channel {channel}" : "Could not connect to the specified device.", exception);
                if (throwExceptions) throw ex;
            }

            return (comm, channel, ex);
        }

        /// <summary>
        /// Connects to the specified devices and returns their CommManagers.连接到指定的设备并返回它们的commmanager。
        /// </summary>
        /// <param name="devices">The devices.</param>
        /// <returns>
        /// The CommManagers
        /// </returns>
        /// <exception cref="ArgumentNullException">The specified devices cannot be null.</exception>
        internal Task<(CommManager Comm, int ChannelIndex, Exception Exception)[]> ConnectAsync(Device[] devices, int[] channelIndices)
        {
            if (devices == null)
                throw new ArgumentNullException("The specified devices cannot be null.指定的设备不能为空");

            if (channelIndices == null)
            {
                channelIndices = new int[devices.Length];
                for (int i = 0; i < devices.Length; i++)
                    channelIndices[i] = i;
            }

            return PalmSens.Core.Simplified.PSMultiCommSimple.GetTaskResultsAndOrExceptions(
                (int channel) => ConnectAsync(devices[channel], channel, false), channelIndices);
        }

        /// <summary>
        /// Disconnects the device using its CommManager.使用CommManager断开设备连接。
        /// </summary>
        /// <param name="comm">The device's CommManager.</param>
        /// <exception cref="System.ArgumentNullException">The specified CommManager cannot be null.</exception>
        internal void Disconnect(CommManager comm)
        {
            if (comm == null)
                throw new ArgumentNullException("The specified CommManager cannot be null.");

            TaskCompletionSource<bool> tcs = new TaskCompletionSource<bool>();
            EventHandler disconnected = (sender, args) => tcs.SetResult(true);
            comm.Disconnected += disconnected;
            comm.DisconnectAsync();
            tcs.Task.Wait();
            comm.Disconnected -= disconnected;
        }

        /// <summary>
        /// Disconnects the device using its CommManager.使用CommManager断开设备连接。
        /// </summary>
        /// <param name="comm">The device's CommManager.</param>
        /// <exception cref="System.ArgumentNullException">The specified CommManager cannot be null.</exception>
        internal async Task<(int channelIndex, Exception ex)> DisconnectAsync(CommManager comm, bool throwException = true)
        {
            Exception ex = null;
            int index = -1;
            if (comm == null)
                ex = new ArgumentNullException("The specified CommManager cannot be null.");
            else
            {
                index = comm.ChannelIndex;
                try
                {
                    await comm.DisconnectAsync();
                }
                catch (Exception exception)
                {
                    ex = new Exception(string.Format("Failed to connect{0}.", index == -1 ? "" : "to channel " + index.ToString()), exception);
                    if (throwException) throw ex;
                }
            }

            return (index, ex);
        }

        /// <summary>
        /// Disconnects the devices using their CommManagers.使用设备的commmanager断开设备连接。
        /// </summary>
        /// <param name="comms">The comms.</param>
        /// <exception cref="System.ArgumentNullException">The specified CommManager array cannot be null.</exception>
        internal async Task<IEnumerable<(int channelIndex, Exception ex)>> Disconnect(IEnumerable<CommManager> comms, bool throwExceptions = true)
        {
            if (comms == null)
                throw new ArgumentNullException("The specified CommManager array cannot be null.");
            List<Task<(int, Exception)>> disconnectTasks = new List<Task<(int, Exception)>>();
            foreach (CommManager comm in comms)
                disconnectTasks.Add(DisconnectAsync(comm, false));
            IEnumerable<(int channelIndex, Exception exception)> result = await Task.WhenAll(disconnectTasks);

            List<Exception> exceptions = new List<Exception>();
            foreach (var channel in result)
            {
                if (channel.exception != null)
                    exceptions.Add(new Exception(string.Format("Failed to disconnect{0}.", channel.channelIndex == -1 ? "" : "to channel " + channel.channelIndex.ToString()), channel.exception));
            }

            if (throwExceptions && exceptions.Count > 0)
                throw new AggregateException(exceptions);

            return result;
        }
    }
}
