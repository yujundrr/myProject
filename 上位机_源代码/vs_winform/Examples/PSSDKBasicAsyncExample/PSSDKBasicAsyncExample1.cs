using System;
using System.Drawing;
using System.Threading.Tasks;
using System.Windows.Forms;
using PalmSens;
using PalmSens.Comm;
using PalmSens.Core.Simplified.Data;
using PalmSens.Devices;
using PalmSens.Techniques;


namespace PSSDKBasicAsyncExample
{
    public partial class PSSDKBasicAsyncExample1 : Form
    {
        
        public PSSDKBasicAsyncExample1()
        {
            InitializeComponent();

            /*PSSDKBasicAsyncExample1.psCommSimpleWinForms.EnableBluetooth = false;
            PSSDKBasicAsyncExample1.psCommSimpleWinForms.EnableSerialPort = false;
            PSSDKBasicAsyncExample1.psCommSimpleWinForms.Parent = null;*/
            PSSDKBasicAsyncExample1.psCommSimpleWinForms.EnableBluetooth = false;
            PSSDKBasicAsyncExample1.psCommSimpleWinForms.EnableSerialPort = true;
            PSSDKBasicAsyncExample1.psCommSimpleWinForms.Parent = this;
            PSSDKBasicAsyncExample1.psCommSimpleWinForms.ReceiveStatus += new PalmSens.Comm.StatusEventHandler(this.psCommSimpleWinForms_ReceiveStatus);
            PSSDKBasicAsyncExample1.psCommSimpleWinForms.MeasurementStarted += new System.EventHandler(this.psCommSimpleWinForms_MeasurementStarted);
            PSSDKBasicAsyncExample1.psCommSimpleWinForms.MeasurementEnded += new System.EventHandler<System.Exception>(this.psCommSimpleWinForms_MeasurementEnded);
            PSSDKBasicAsyncExample1.psCommSimpleWinForms.StateChanged += new PalmSens.Comm.CommManager.StatusChangedEventHandler(this.psCommSimpleWinForms_StateChanged);
            PSSDKBasicAsyncExample1.psCommSimpleWinForms.Disconnected += new PalmSens.Core.Simplified.DisconnectedEventHandler(this.psCommSimpleWinForms_Disconnected);
            PSSDKBasicAsyncExample1.psCommSimpleWinForms.SimpleCurveStartReceivingData += new PalmSens.Core.Simplified.PSCommSimple.SimpleCurveStartReceivingDataHandler(this.psCommSimpleWinForms_SimpleCurveStartReceivingData);

            InitLSVMethod(); //Create the linear sweep voltammetry method that defines the measurement parameters
            InitDataGrid(); //Set up the columns for the datagridview control
            InitPlot(); //Resets and initiates the plot control
            DiscoverConnectedDevicesAsync(); //Populate the connected device combobox control
        }


        public static Device[] _connectedDevices = new Device[0];
        /// <summary>
        /// The instance of method class containing the Linear Sweep Voltammetry parameters
        /// </summary>
        private LinearSweep _methodLSV;

        /// <summary>
        /// The connected PalmSens & EmStat devices
        /// </summary>
        

        /// <summary>
        /// The active SimpleMeasurement
        /// </summary>
        private SimpleMeasurement _activeMeasurement = null;


        private void InitPlot()
        {
            plot.ClearAll(); //Clear all curves and data from plot
            //Set the Axis labels
            plot.XAxisLabel = "Potential/V";
            plot.YAxisLabel = "Current/µA";
            plot.AddData("", new double[0], new double[0]); //Add a empty data array to draw an empty plot
        }
        /// <summary>
        /// Initializes the data grid view control.初始化数据网格视图控件。
        /// </summary>
        private void InitDataGrid()
        {
            dgvMeasurement.Rows.Clear();
            dgvMeasurement.Columns.Clear();

            DataGridViewTextBoxColumn dgvColID = new DataGridViewTextBoxColumn();
            dgvColID.HeaderText = "ID";
            dgvColID.AutoSizeMode = DataGridViewAutoSizeColumnMode.AllCells;
            dgvColID.ReadOnly = true;

            DataGridViewTextBoxColumn dgvColPotential = new DataGridViewTextBoxColumn();
            dgvColPotential.HeaderText = "Potential (V)";
            dgvColPotential.AutoSizeMode = DataGridViewAutoSizeColumnMode.AllCells;
            dgvColPotential.ReadOnly = true;

            DataGridViewTextBoxColumn dgvColCurrent = new DataGridViewTextBoxColumn();
            dgvColCurrent.HeaderText = "Current (µA)";
            dgvColCurrent.AutoSizeMode = DataGridViewAutoSizeColumnMode.Fill;
            dgvColCurrent.ReadOnly = true;

            dgvMeasurement.Columns.Add(dgvColID);
            dgvMeasurement.Columns.Add(dgvColPotential);
            dgvMeasurement.Columns.Add(dgvColCurrent);
        }

        /// <summary>
        /// Initializes the LSV method.
        /// </summary>
        private void InitLSVMethod()
        {
            tbBegin.Text = Convert.ToString(beginV);
            tbEnd.Text = Convert.ToString(endV);
            tbStep.Text = Convert.ToString(stepV);
            tbRate.Text = Convert.ToString(rateV);
            tbEquilib.Text = Convert.ToString(equilibT);

            _methodLSV = new LinearSweep(); //Create a new linear sweep method with the default settings使用默认设置创建一个新的线性扫描方法
            _methodLSV.BeginPotential = Convert.ToSingle(beginV); //Sets the potential to start the sweep from设置开始扫描的电位
            _methodLSV.EndPotential = Convert.ToSingle(endV); //Sets the potential for the sweep to stop at设置扫描停止的电位
            _methodLSV.StepPotential = Convert.ToSingle(stepV); //Convert.ToSingle(endV); //Sets the step size设置步长
            _methodLSV.Scanrate = Convert.ToSingle(rateV); //Sets the scan rate to 0.1 V/s设置扫描速率为0.1 V/s

            _methodLSV.EquilibrationTime = Convert.ToSingle(equilibT); //Equilabrates the cell at the defined potential for 1 second before starting the measurement在开始测量前，使电池在定义的电位下平衡1秒
            _methodLSV.Ranging.StartCurrentRange = new CurrentRange(CurrentRanges.cr1uA); //Starts equilabration in the 1µA current range在1uA电流范围内开始均衡
            _methodLSV.Ranging.MinimumCurrentRange = new CurrentRange(CurrentRanges.cr100nA); //Min current range 10nA最小电流范围10nA
            _methodLSV.Ranging.MaximumCurrentRange = new CurrentRange(CurrentRanges.cr5mA); //Max current range 1mA最大电流范围1mA
            _methodLSV.SaveOnDevice = false;
            _methodLSV.ToMethodScript(new EmStatPicoCapabilities());
        }

        /// <summary>
        /// Discovers the connected PalmSens & EmStat devices and adds them to the combobox control.
        /// </summary>
        public static async Task connectedDevices()
        {
            _connectedDevices = await psCommSimpleWinForms.GetConnectedDevicesAsync(); //Discover connected devices

        }
        private async Task DiscoverConnectedDevicesAsync()
        {
            btnRefresh.Enabled = false;
            lbConsole.Items.Add("正在搜索可用设备。");
            cmbDevices.Items.Clear();
            connectedDevices(); //Discover connected devices

            foreach (Device d in _connectedDevices) 
                cmbDevices.Items.Add(d.ToString()); //Add connected devices to control

            int nDevices = cmbDevices.Items.Count;
            cmbDevices.SelectedIndex = nDevices > 0 ? 0 : -1;
            lbConsole.Items.Add($"发现 {nDevices} 台设备");

            btnConnect.Enabled = nDevices > 0;
            btnRefresh.Enabled = true;
        }

        /// <summary>
        /// Handles the Click event of the btnRefresh control.
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="EventArgs"/> instance containing the event data.</param>
        private async void btnRefresh_Click(object sender, EventArgs e)
        {
            connectedDevices();
            await DiscoverConnectedDevicesAsync(); //Add connected devices to the devices combobox control
        }

        /// <summary>
        /// Handles the Click event of the btnConnect control.
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="EventArgs"/> instance containing the event data.</param>
        private async void btnConnect_Click(object sender, EventArgs e)
        {
            btnConnect.Enabled = false;
            if (!psCommSimpleWinForms.Connected) //Determine whether a device is currently connected判断设备当前是否已连接
            {
                if (cmbDevices.SelectedIndex == -1)
                {
                    lbConsole.Items.Add($"无可用设备");
                    return;
                }
                    

                try
                {
                    //Connect to the device selected in the devices combobox control
                    await psCommSimpleWinForms.ConnectAsync(_connectedDevices[cmbDevices.SelectedIndex]); 
                    lbConsole.Items.Add($"已经连接到 {psCommSimpleWinForms.ConnectedDevice.ToString()}");
                }
                catch (Exception ex)
                {
                    lbConsole.Items.Add(ex.Message);
                }
            }
            else
            {
                await psCommSimpleWinForms.DisconnectAsync(); //Disconnect from the connected device等待断开连接
            }

            //Update UI based on connection status
            cmbDevices.Enabled = !psCommSimpleWinForms.Connected;
            btnRefresh.Enabled = !psCommSimpleWinForms.Connected;
            btnConnect.Text = psCommSimpleWinForms.Connected ? "断开连接" : "连接";
            btnMeasure.Enabled = psCommSimpleWinForms.Connected;
            btnConnect.Enabled = true;
        }

        /// <summary>
        /// Handles the Click event of the btnMeasure control.
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="EventArgs"/> instance containing the event data.</param>
        private async void btnMeasure_Click(object sender, EventArgs e)
        {
            btnMeasure.Enabled = false;
            if (psCommSimpleWinForms.DeviceState == PalmSens.Comm.CommManager.DeviceState.Idle) //Determine whether the device is currently idle or measuring
            {
                try
                {
                    plot.ClearAll(); //Clears data from previous measurements from the plot

                    InitDataGrid(); //Reset the data grid view control
                    InitLSVMethod();
                    _activeMeasurement = await psCommSimpleWinForms.MeasureAsync(_methodLSV); //Start measurement defined in the method
                }
                catch (Exception ex)
                {
                    lbConsole.Items.Add(ex.Message);
                }
            }
            else
            {
                try
                {
                    await psCommSimpleWinForms.AbortMeasurementAsync(); //Abort the active measurement
                }
                catch (Exception ex)
                {
                    lbConsole.Items.Add(ex.Message);
                }
            }
            btnMeasure.Enabled = true;
        }

        /// <summary>
        /// Raised when device status package is received (the device does not send status packages while measuring)
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="PalmSens.Comm.StatusEventArgs"/> instance containing the event data.</param>包含事件数据的PalmSens.Comm.StatusEventArgs e 实例。
        private async void psCommSimpleWinForms_ReceiveStatus(object sender, PalmSens.Comm.StatusEventArgs e)
        {
            Status status = e.GetStatus(); //Get the PalmSens.Comm.Status instance from the event data
            double potential = status.PotentialReading.Value; //Get the potential
            double currentInRange = status.CurrentReading.ValueInRange; //Get the current expressed inthe active current range
            PalmSens.Comm.ReadingStatus currentStatus = status.CurrentReading.ReadingStatus; //Get the status of the current reading
            CurrentRange cr = status.CurrentReading.CurrentRange; //Get the active current range

            tbPotential.Text = potential.ToString("F3");
            tbCurrent.Text = currentInRange.ToString("F3");
            switch (currentStatus)
            {
                case PalmSens.Comm.ReadingStatus.OK:
                    tbCurrent.ForeColor = Color.Black;
                    break;
                case PalmSens.Comm.ReadingStatus.Overload:
                    tbCurrent.ForeColor = Color.Red;
                    break;
                case PalmSens.Comm.ReadingStatus.Underload:
                    tbCurrent.ForeColor = Color.Yellow;
                    break;
            }
            lblCurrentRange.Text = $"* {cr.ToString()}";
        }

        /// <summary>
        /// Raised when the connected device's status changes
        /// </summary>
        /// <param name="sender">The sender.</param>
        /// <param name="CurrentState">State of the current.</param>
        private void psCommSimpleWinForms_StateChanged(object sender, PalmSens.Comm.CommManager.DeviceState CurrentState)
        {
            tbDeviceStatus.Text = CurrentState.ToString(); //Updates the device state indicator textbox
            btnConnect.Enabled = CurrentState == PalmSens.Comm.CommManager.DeviceState.Idle;
            btnMeasure.Text = CurrentState == PalmSens.Comm.CommManager.DeviceState.Idle ? "开始测量" : "取消测量";
        }

        /// <summary>
        /// Raised when the measurement is ended当测量结束时触发
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="EventArgs"/> instance containing the event data.</param>
        private void psCommSimpleWinForms_MeasurementEnded(object sender, Exception e)
        {
            lbConsole.Items.Add("测量结束");
        }

        /// <summary>
        /// Raised when the measurement is started当测量开始时引发
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="EventArgs"/> instance containing the event data.</param>
        private void psCommSimpleWinForms_MeasurementStarted(object sender, EventArgs e)
        {
            lbConsole.Items.Add("线性扫描法（LSV）测量开始.");
        }

        /// <summary>
        /// Raised when a Simple Curve in the active SimpleMeasurement starts receiving data当活动simplemmeasurement中的简单曲线开始接收数据时引发
        /// </summary>
        /// <param name="sender">The sender.</param>
        /// <param name="activeSimpleCurve">The active simple curve.</param>
        private void psCommSimpleWinForms_SimpleCurveStartReceivingData(object sender, SimpleCurve activeSimpleCurve)
        {
            plot.AddSimpleCurve(activeSimpleCurve);
            //Subscribe to the curve's events to receive updates when new data is available and when it iss finished receiving data
            //订阅曲线的事件，以便在新数据可用和接收数据完成时接收更新
            activeSimpleCurve.NewDataAdded += activeSimpleCurve_NewDataAdded;
            activeSimpleCurve.CurveFinished += activeSimpleCurve_CurveFinished;

            lbConsole.Items.Add("正在接收新的数据...");
        }

        /// <summary>
        /// Raised when new data points are added to the active SimpleCurve
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="PalmSens.Data.ArrayDataAddedEventArgs"/> instance containing the event data.</param>
        private void activeSimpleCurve_NewDataAdded(object sender, PalmSens.Data.ArrayDataAddedEventArgs e)
        {
            if (InvokeRequired) //Data is parsed asynchronously in the case this event was raised on a different thread it must be invoked back onto the UI thread
            {
                BeginInvoke(new PalmSens.Plottables.Curve.NewDataAddedEventHandler(activeSimpleCurve_NewDataAdded), sender, e);
                return;
            }
            SimpleCurve activeSimpleCurve = sender as SimpleCurve;
            int startIndex = e.StartIndex; //The index of the first new data point added to the curve
            int count = e.Count; //The number of new data points added to the curve

            for (int i = startIndex; i < startIndex + count; i++)
            {
                double xValue = activeSimpleCurve.XAxisValue(i); //Get the value on Curve's X-Axis (potential) at the specified index获取曲线x轴上指定索引处的值(势)
                double yValue = activeSimpleCurve.YAxisValue(i); //Get the value on Curve's Y-Axis (current) at the specified index获取曲线y轴(current)上指定索引处的值
                dgvMeasurement.Rows.Add(1);
                dgvMeasurement.Rows[i].Cells[0].Value = (i + 1).ToString();
                dgvMeasurement.Rows[i].Cells[1].Value = xValue.ToString("F2");
                dgvMeasurement.Rows[i].Cells[2].Value = yValue.ToString("E3");
            }

            tbPotential.Text = activeSimpleCurve.XAxisValue(startIndex + count - 1).ToString("F3");
            tbCurrent.Text = activeSimpleCurve.YAxisValue(startIndex + count - 1).ToString("F3");
        }

        /// <summary>
        /// Raised when a SimpleCurve stops receiving new data points当SimpleCurve停止接收新数据点时引发
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="EventArgs"/> instance containing the event data.</param>
        private void activeSimpleCurve_CurveFinished(object sender, EventArgs e)
        {
            if (InvokeRequired) //Data is parsed asynchronously in the case this event was raised on a different thread it must be invoked back onto the UI thread如果在不同的线程上引发此事件，则数据将被异步解析，必须将其调用回UI线程
            {
                BeginInvoke(new EventHandler(activeSimpleCurve_CurveFinished), sender, e);
                return;
            }
            SimpleCurve activeSimpleCurve = sender as SimpleCurve;

            //Unsubscribe from the curves events to avoid memory leaks取消订阅曲线事件以避免内存泄漏
            activeSimpleCurve.NewDataAdded -= activeSimpleCurve_NewDataAdded;
            activeSimpleCurve.CurveFinished -= activeSimpleCurve_CurveFinished;

            int nDataPointsReceived = activeSimpleCurve.NDataPoints;
            lbConsole.Items.Add($"一共接收 {nDataPointsReceived}  数据点.");

            lbConsole.Items.Add("数据曲线绘图完成");
        }

        /// <summary>
        /// Raised when the instrument has been disconnected.当仪器断开时触发。
        /// If the instrument was disconnected due to a communication the exception is provided.
        /// In the case of a regular disconnect the exception will be set to null.
        /// </summary>
        /// <param name="sender">The sender.</param>
        /// <param name="exception">The exception.</param>
        private void psCommSimpleWinForms_Disconnected(object sender, Exception exception)
        {
            if (exception != null)
            {
                lbConsole.Items.Add(exception.Message);
            }

            lbConsole.Items.Add("设备已断开连接");
            btnConnect.Text = "连接";
            btnConnect.Enabled = true;
            btnMeasure.Text = "开始测量";
        }

        private void lbConsole_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void tbBegin_TextChanged(object sender, EventArgs e)
        {
            string strValue = tbBegin.Text;
            if (strValue == "")
            {

                return;
            }
            float intValue = Convert.ToSingle(strValue);
            //lbConsole.Items.Add($"{intValue }");
            if (intValue > 2 || intValue < -2)
            {
                lbConsole.Items.Add("输入的参数不正确，");
                lbConsole.Items.Add("起始电位最大值为2V");
                MessageBox.Show("输入的参数不正确，起始电位最大值为2V");
                tbBegin.Text = "";

                return;
            }

            if (intValue == endV && intValue == 0)
            {
                lbConsole.Items.Add("输入的参数不正确，");
                lbConsole.Items.Add("起始电位和终止电位相同。");
                MessageBox.Show("输入的参数不正确，起始电位和终止电位相同。");
                tbBegin.Text = "";

                return;
            }

            beginV = intValue;

        }

        private void tbEnd_TextChanged(object sender, EventArgs e)
        {
            string strValue = tbEnd.Text;
            if (strValue == "")
            {

                return;
            }
            float intValue = Convert.ToSingle(strValue);
            if (intValue > 2 || intValue < -2 )
            {
                lbConsole.Items.Add("输入的参数不正确，");
                lbConsole.Items.Add("起始电位最大值为2V。");
                MessageBox.Show("输入的参数不正确，起始电位最大值为2V。");
                tbEnd.Text = "";

                return;
            }
            if (intValue == beginV && intValue == 0)
            {
                lbConsole.Items.Add("输入的参数不正确，");
                lbConsole.Items.Add("起始电位和终止电位相同。");
                MessageBox.Show("输入的参数不正确，起始电位和终止电位相同。");
                tbEnd.Text = "";

                return;
            }
            endV = intValue;

        }

        private void tbStep_TextChanged(object sender, EventArgs e)
        {
            string strValue = tbStep.Text;
            if (strValue == "")
            {

                return;
            }
            float intValue = Convert.ToSingle(strValue);
            if (intValue > 2 || intValue < -2)
            {
                lbConsole.Items.Add("输入的参数不正确，");
                lbConsole.Items.Add("最大步进电位为0.250V。");
                MessageBox.Show("输入的参数不正确，最大步进电位为0.250V。");
                tbStep.Text = "";

                return;
            }

            stepV = intValue;
        }

        private void tbRate_TextChanged(object sender, EventArgs e)
        {
            string strValue = tbRate.Text;
            if (strValue == "")
            {

                return;
            }
            float intValue = Convert.ToSingle(strValue);
            if (intValue > 2 || intValue < -2)
            {
                lbConsole.Items.Add("输入的参数不正确，");
                lbConsole.Items.Add("测量速率太快，");
                lbConsole.Items.Add("无法自动调整测量范围。");
                lbConsole.Items.Add("且最大测量速率为每秒1000个数据点。");
                MessageBox.Show("输入的参数不正确，测量速率太快，无法自动调整测量范围。且最大测量速率为每秒1000个数据点。");
                tbRate.Text = "";

                return;
            }

            rateV = intValue;
        }

        private void tbEquilib_TextChanged(object sender, EventArgs e)
        {
            string strValue = tbEquilib.Text;
            if (strValue == "")
            {

                return;
            }
            float intValue = Convert.ToSingle(strValue);

            equilibT = intValue;
        }
   
    }
}
