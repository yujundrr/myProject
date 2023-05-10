using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using PalmSens.Comm;
using PalmSens.Core.Simplified.WinForms.DeviceFirmware;
using PalmSens.Devices;
using PSSDKBasicAsyncExample;

//using impedance_winform;

namespace PSSDKFirmwareExample
{
    public partial class FrmFirmware : Form
    {
        /// <summary>
        /// The firmware extensions固件的扩展
        /// </summary>
        private const string FileListExtension = "Firmware Files (*.hex, *.bin)|*.hex;*.bin|All Files (*.*)|*.*";
        private Device[] _connectedDevices = new Device[0];
        public static StringBuilder _currentDeviceInfo;
        private bool _discovering;
        private IFirmwareManager _firmwareManager;
        private enumDeviceType _selectedDeviceType;
        private bool _updating;

        public FrmFirmware()
        {
            InitializeComponent();

            PSSDKBasicAsyncExample.PSSDKBasicAsyncExample1.psCommSimpleWinForms.EnableBluetooth = false;
            PSSDKBasicAsyncExample.PSSDKBasicAsyncExample1.psCommSimpleWinForms.EnableSerialPort = false;
            PSSDKBasicAsyncExample.PSSDKBasicAsyncExample1.psCommSimpleWinForms.Parent = this;
            PSSDKBasicAsyncExample.PSSDKBasicAsyncExample1.psCommSimpleWinForms.Disconnected += new PalmSens.Core.Simplified.DisconnectedEventHandler(this.psCommSimpleWinForms_Disconnected);
            
            _connectedDevices = PSSDKBasicAsyncExample1._connectedDevices;
            //_currentDeviceInfo = new StringBuilder();
        }

        private void btnLoadFile_Click(object sender, EventArgs e)
        {
            // Open file dialog
            using (var fileDialog = new OpenFileDialog())
            {
                // Specify that the file must exist.指定该文件必须存在。
                fileDialog.CheckPathExists = true;
                // Specify extensions filter指定扩展过滤器
                fileDialog.Filter = FileListExtension;

                // Show file dialog显示文件对话框
                var result = fileDialog.ShowDialog(this);

                if (result == DialogResult.OK)
                {
                    // If a file has been selected. create the firmware manager.如果选中了某个文件。创建固件管理器。
                    txtFileLocation.Text = fileDialog.FileName;
                    _firmwareManager = FirmwareManager.CreateManager(fileDialog.FileName);
                }
            }

            UpdateFirmWareInfo();
            UpdateFormControls();
            UpdateStatus(FirmwareUploadStatus.None);
            UpdateDownloadProgress(0, 1);
        }

        private async void btnRefresh_Click(object sender, EventArgs e)
        {
            await DiscoverConnectedDevicesAsync();
        }

        private async void btnUpload_Click(object sender, EventArgs e)
        {
            // Do nothing if no firmware manager or selected device.如果没有固件管理器或选择的设备，则不做任何操作。
            if (_firmwareManager == null || cmbDevices.SelectedIndex < 0) return;

            // Validate that the selected device can be used by the firmware.确认所选设备可以被固件使用。
            if (!_firmwareManager.Validate(_selectedDeviceType))
            {
                // Log and show an error message when the device is not supported.
                //当设备不被支持时，记录并显示错误消息。
                var message = $"所选设备 {_selectedDeviceType} 不被固件支持。";
                LogMessage(message);
                MessageBox.Show(message, "", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return;
            }

            // Update the controls for the form.更新表单的控件。
            _updating = true;
            UpdateFormControls();
            IDeviceFirmwareUploader uploader = null;

            try
            {
                // Get the selected device.获取选定的设备。
                var device = _connectedDevices[cmbDevices.SelectedIndex];
                // Set the device in download mode and get its uploader.将设备设置为下载模式并获取其上传器。
                uploader = await _firmwareManager.SetDeviceInDownloadModeAndGetDeviceFirmwareUploader(device);
                // Subscribe to the uploader events.订阅上传程序事件。
                uploader.Progress += Uploader_Progress;
                uploader.Message += Uploader_Message;

                // Upload the firmware to the device.上传固件到设备。
                await uploader.UploadAsync();
            }
            catch(Exception exception)
            {
                // Log and display any errors that have occurred.//记录并显示已发生的任何错误。
                LogException(exception);
                MessageBox.Show($"在尝试上传新固件时发生错误, {exception}");
            }
            finally
            {
                _updating = false;
                // Discover devices, this will display the updated device information.
                //发现设备，这将显示更新的设备信息。
                await DiscoverConnectedDevicesAsync();
                UpdateFormControls();
                UpdateFirmWareInfo();
                UpdateStatus(FirmwareUploadStatus.None);
                // Unsubscribe from the uploader and dispose.
                //从上传器取消订阅并销毁。
                if (uploader != null)
                {
                    uploader.Progress -= Uploader_Progress;
                    uploader.Message -= Uploader_Message;
                    uploader.Dispose();
                }
            }
        }

        private async void FrmFirmware_Load(object sender, EventArgs e)
        {
            // On form load, discover connected devices.在表单加载时，发现连接的设备。
            await DiscoverConnectedDevicesAsync();
            // Update the status and download status to default.更新状态和下载状态到默认。
            UpdateStatus(FirmwareUploadStatus.None);
            UpdateDownloadProgress(0, 0);
        }

        private void psCommSimpleWinForms_Disconnected(object sender, Exception commErrorException)
        {
            // Update form when device disconnects当设备断开连接时更新表单
            LogMessage("Disconnected from device.");

            // Log any errors//记录所有错误
            if (commErrorException != null) LogException(commErrorException);

            UpdateFormControls();
        }

        private void Uploader_Message(object sender, DeviceFirmwareUploaderMessageEventArgs e)
        {
            // Log uploader messages and status.//日志上传者消息和状态。
            LogMessage(e.ToString());
            UpdateStatus(e.Status);
        }

        private void Uploader_Progress(object sender, DeviceFirmwareUploaderProgressEventArgs e)
        {
            // Update the uploader status.//更新上传状态。
            UpdateDownloadProgress(e.Current, e.Total);
            UpdateStatus(e.Status);
        }

        private async Task DisconnectAsync()
        {
            await PSSDKBasicAsyncExample1.psCommSimpleWinForms.DisconnectAsync();
        }

        /// <summary>
        /// Update the download progress更新下载进度
        /// </summary>
        /// <param name="current">The current amount.</param>
        /// <param name="total">The total.</param>
        private void UpdateDownloadProgress(int current, int total)
        {
            if (InvokeRequired)
            {
                Invoke((Action<int, int>) UpdateDownloadProgress, current, total);
                return;
            }

            pgsBarUpload.Maximum = total;
            pgsBarUpload.Value = current;
        }

        /// <summary>
        /// Update the status of the uploader.更新上传程序的状态。
        /// </summary>
        /// <param name="status"></param>
        private void UpdateStatus(FirmwareUploadStatus status)
        {
            if (InvokeRequired)
            {
                Invoke((Action<FirmwareUploadStatus>) UpdateStatus, status);
                return;
            }

            lblCurrentStatus.Text = status == FirmwareUploadStatus.None ? "" : status.ToString();
        }

        /// <summary>
        ///     Discovers the connected PalmSens & EmStat devices and adds them to the combobox control.
        ///     发现连接的PalmSens & EmStat设备，并将它们添加到组合框控件中。
        /// </summary>
        private async Task DiscoverConnectedDevicesAsync()
        {
            // Ignore if connected to a device.忽略如果连接到一个设备。
            if (PSSDKBasicAsyncExample1.psCommSimpleWinForms.Connected)
            {
                LogMessage("设备已连接.");
                UpdateFirmWareInfo();
                return;
            }
               

            // Clear controls
            cmbDevices.Items.Clear();
            //_connectedDevices.Clear();

            LogMessage("搜索可用设备.");
            
            // Update form controls, prevent interaction
            _discovering = true;
            UpdateFormControls();
            btnRefresh.Enabled = false;

            try
            {
                // Get connected devices list.
                var devices = _connectedDevices; //Discover connected devices

                // Add the devices
                foreach (var device in devices)
                {
                    //_connectedDevices.Add(device);
                    cmbDevices.Items.Add(device.ToString()); //Add connected devices to control
                }

                // Set the initial selection
                var nDevices = cmbDevices.Items.Count;
                cmbDevices.SelectedIndex = nDevices > 0 ? 0 : -1;

                LogMessage($"Found {nDevices} device(s).");
            }
            finally
            {
                // Reset controls
                UpdateFormControls();
                _discovering = false;
            }

            // Get device info
            await GetDeviceInfo();
        }

        private void UpdateFormControls()
        {
            if (InvokeRequired)
            {
                Invoke((Action) UpdateFormControls);
                return;
            }

            // Get an indicator if the comms is connected.如果通讯已连接，请准备指示灯。
            var connected = PSSDKBasicAsyncExample1.psCommSimpleWinForms.Connected;

            // Enable refresh button if not connected or updating.如果没有连接或更新，启用刷新按钮。
            btnRefresh.Enabled = !connected && !_updating;
            // Enable upload button if valid device, has selected device and firmware selected.
            //启用上传按钮，如果有效的设备，已选择的设备和固件选择。
            btnUpload.Enabled = cmbDevices.Items.Count > 0 && cmbDevices.SelectedIndex >= 0 && _firmwareManager != null && !_updating && CheckDeviceAndWarn(_selectedDeviceType);
        }

        /// <summary>
        /// Update the firmware info更新固件信息
        /// </summary>
        public StringBuilder UpdateFirmWareInfo()
        {
            var builder = new StringBuilder();

            // Update the current selected device info if selected.
            if (_currentDeviceInfo.Length > 0) builder.AppendLine(_currentDeviceInfo.ToString());
            // Update the current firmware info if selected.
            if (_firmwareManager != null) builder.Append(_firmwareManager);

            // Update firmware text with all info
            txtFirmware.Text = builder.ToString().Trim();

            return builder;
        }

        /// <summary>
        /// Log a message to the log.将消息记录到日志。
        /// </summary>
        /// <param name="message"></param>
        private void LogMessage(string message)
        {
            if (InvokeRequired)
            {
                Invoke((Action<string>) LogMessage, message);
                return;
            }

            // Log the message to the console.
            lbConsole.SelectedIndex = lbConsole.Items.Add(message);
        }

        /// <summary>
        /// Log an error记录一个错误
        /// </summary>
        /// <param name="e">The error to log.</param>
        /// <returns></returns>
        private bool LogException(Exception e)
        {
            LogMessage(e.ToString());
            return true;
        }

        /// <summary>
        /// Connect to the device.连接设备。
        /// </summary>
        /// <returns></returns>
        private async Task ConnectAsync()
        {
            // Ignore if already connected.
            if (PSSDKBasicAsyncExample1.psCommSimpleWinForms.Connected) return;

            try
            {
                // Get the selected device.
                var device = _connectedDevices[cmbDevices.SelectedIndex];
                // Connect to the device
                await PSSDKBasicAsyncExample1.psCommSimpleWinForms.ConnectAsync(device);

                LogMessage($"已经连接到 {PSSDKBasicAsyncExample1.psCommSimpleWinForms.ConnectedDevice}");
            }
            // Handle any exceptions that occur
            catch (AggregateException a)
            {
                a.Handle(LogException);
            }
            catch (Exception exc)
            {
                LogException(exc);
            }
        }

        /// <summary>
        /// Handle device selection.处理设备的选择。
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private async void cmbDevices_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (_updating || _discovering) return;

            try
            {
                // Get the device info
                await GetDeviceInfo();
            }
            // Handle any exceptions that occur
            catch (Exception exception)
            {
                LogException(exception);
            }
        }

        public async Task GetDeviceInfo()
        {
            cmbDevices.Enabled = false;
            btnUpload.Enabled = false;
            // Clear the device info builder.
            _currentDeviceInfo.Clear();
            // Reset the selected device.
            _selectedDeviceType = enumDeviceType.None;
            try
            {
                // Connect if a device is selected.
                if (cmbDevices.SelectedIndex < 0) return;
            
                // Connect
                await ConnectAsync();

                if (PSSDKBasicAsyncExample1.psCommSimpleWinForms.Connected)
                {
                    // Get the connected device type获取连接的设备类型
                    _selectedDeviceType = PSSDKBasicAsyncExample1.psCommSimpleWinForms.ConnectedDevice;

                    // Validate if the device type is supported and warn, do not prevent interaction.
                    //验证设备类型是否支持，并发出警告，不阻止交互。
                    if (!CheckDeviceAndWarn(_selectedDeviceType)) LogMessage($"The device {_selectedDeviceType} is currently not supported.");

                    // Check of the device is a MethodScript device, they currently only devices that use the binary format.
                    //检查的设备是一个MethodScript设备，它们目前只使用二进制格式的设备。
                    var isMsDevice = PSSDKBasicAsyncExample1.psCommSimpleWinForms.Comm.ClientConnection is ClientConnectionMS;

                    // Create device info创建设备信息
                    _currentDeviceInfo.AppendLine("设备信息:")
                        .Append("   连接设备: ").Append(PSSDKBasicAsyncExample1.psCommSimpleWinForms.ConnectedDevice).AppendLine()
                        .Append("   固件版本: ").AppendLine(PSSDKBasicAsyncExample1.psCommSimpleWinForms.Capabilities.FirmwareVersion.ToString(isMsDevice ? "0.000" : "0.0"))
                        .Append("   固件下载日期: ").Append(PSSDKBasicAsyncExample1.psCommSimpleWinForms.Capabilities.FirmwareTimeStamp);

                    if (!string.IsNullOrEmpty(PSSDKBasicAsyncExample1.psCommSimpleWinForms.Capabilities.SpecialFirmwareDescription))
                        _currentDeviceInfo.AppendLine()
                            .Append($"   特殊的固件描述: {PSSDKBasicAsyncExample1.psCommSimpleWinForms.Capabilities.SpecialFirmwareDescription}");
                }
            }
            finally
            {
                // Disconnect if needed.
                await DisconnectAsync();

                // Update form details.
                UpdateFirmWareInfo();
                UpdateFormControls();
                UpdateStatus(FirmwareUploadStatus.None);
                UpdateDownloadProgress(0, 1);
                cmbDevices.Enabled = true;
            }
        }

        public bool CheckDeviceAndWarn(enumDeviceType deviceType)
        {
            // The list below is the current list of untested devices.
            switch (deviceType)
            {
                case enumDeviceType.Unknown:
                case enumDeviceType.None:
                case enumDeviceType.PalmSens:
                case enumDeviceType.EmStat1:
                case enumDeviceType.EmStat2:
                case enumDeviceType.PalmSens3:
                case enumDeviceType.EmStat2BP:
                    return false;
            }

            return true;
        }
    }
}