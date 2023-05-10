namespace PSSDKPlotEISFitAsyncExample
{
    partial class PSSDKPlotEISFitAsyncExample
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.cmbDevices = new System.Windows.Forms.ComboBox();
            this.btnRefresh = new System.Windows.Forms.Button();
            this.btnConnect = new System.Windows.Forms.Button();
            this.btnMeasure = new System.Windows.Forms.Button();
            this.grpConnection = new System.Windows.Forms.GroupBox();
            this.grpDevice = new System.Windows.Forms.GroupBox();
            this.label1 = new System.Windows.Forms.Label();
            this.tbDeviceStatus = new System.Windows.Forms.TextBox();
            this.lblCurrentRange = new System.Windows.Forms.Label();
            this.lblCurrent = new System.Windows.Forms.Label();
            this.tbCurrent = new System.Windows.Forms.TextBox();
            this.lblVolt = new System.Windows.Forms.Label();
            this.tbPotential = new System.Windows.Forms.TextBox();
            this.lblPotential = new System.Windows.Forms.Label();
            this.grpMeasurement = new System.Windows.Forms.GroupBox();
            this.btNyquistZoomin = new System.Windows.Forms.Button();
            this.plotBode = new SDKPlot.WinForms.Plot();
            this.grpConsole = new System.Windows.Forms.GroupBox();
            this.lbConsole = new System.Windows.Forms.ListBox();
            //this.psCommSimpleWinForms = new PalmSens.Core.Simplified.WinForms.PSCommSimpleWinForms(this.components);
            this.grpFit = new System.Windows.Forms.GroupBox();
            this.lblIterations = new System.Windows.Forms.Label();
            this.progressBar = new System.Windows.Forms.ProgressBar();
            this.lbResults = new System.Windows.Forms.ListBox();
            this.btnCancel = new System.Windows.Forms.Button();
            this.btnFit = new System.Windows.Forms.Button();
            this.grpParamSet = new System.Windows.Forms.GroupBox();
            this.label11 = new System.Windows.Forms.Label();
            this.tbFaxFre = new System.Windows.Forms.TextBox();
            this.labelFaxFre = new System.Windows.Forms.Label();
            this.cmbFreType = new System.Windows.Forms.ComboBox();
            this.labelFreType = new System.Windows.Forms.Label();
            this.tbStepFre = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.tbNumFre = new System.Windows.Forms.TextBox();
            this.labelNumFre = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.tbEquilibT = new System.Windows.Forms.TextBox();
            this.labelEquilib = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.tbMinFre = new System.Windows.Forms.TextBox();
            this.tbMaxFre = new System.Windows.Forms.TextBox();
            this.labelMinFre = new System.Windows.Forms.Label();
            this.labelMaxFre = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.labelAC = new System.Windows.Forms.Label();
            this.tbAC = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.tbDC = new System.Windows.Forms.TextBox();
            this.labelDC = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.dgvMeasurement = new System.Windows.Forms.DataGridView();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.btBodeZoomin = new System.Windows.Forms.Button();
            this.plotNyquist = new SDKPlot.WinForms.Plot();
            this.grpConnection.SuspendLayout();
            this.grpDevice.SuspendLayout();
            this.grpMeasurement.SuspendLayout();
            this.grpConsole.SuspendLayout();
            this.grpFit.SuspendLayout();
            this.grpParamSet.SuspendLayout();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dgvMeasurement)).BeginInit();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // cmbDevices
            // 
            this.cmbDevices.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbDevices.FormattingEnabled = true;
            this.cmbDevices.Location = new System.Drawing.Point(6, 18);
            this.cmbDevices.Name = "cmbDevices";
            this.cmbDevices.Size = new System.Drawing.Size(155, 20);
            this.cmbDevices.TabIndex = 0;
            // 
            // btnRefresh
            // 
            this.btnRefresh.Location = new System.Drawing.Point(6, 42);
            this.btnRefresh.Name = "btnRefresh";
            this.btnRefresh.Size = new System.Drawing.Size(75, 21);
            this.btnRefresh.TabIndex = 1;
            this.btnRefresh.Text = "刷新";
            this.btnRefresh.UseVisualStyleBackColor = true;
            this.btnRefresh.Click += new System.EventHandler(this.btnRefresh_Click);
            // 
            // btnConnect
            // 
            this.btnConnect.Enabled = false;
            this.btnConnect.Location = new System.Drawing.Point(86, 42);
            this.btnConnect.Name = "btnConnect";
            this.btnConnect.Size = new System.Drawing.Size(75, 21);
            this.btnConnect.TabIndex = 2;
            this.btnConnect.Text = "连接";
            this.btnConnect.UseVisualStyleBackColor = true;
            this.btnConnect.Click += new System.EventHandler(this.btnConnect_Click);
            // 
            // btnMeasure
            // 
            this.btnMeasure.Enabled = false;
            this.btnMeasure.Location = new System.Drawing.Point(9, 21);
            this.btnMeasure.Name = "btnMeasure";
            this.btnMeasure.Size = new System.Drawing.Size(75, 21);
            this.btnMeasure.TabIndex = 3;
            this.btnMeasure.Text = "开始测量";
            this.btnMeasure.UseVisualStyleBackColor = true;
            this.btnMeasure.Click += new System.EventHandler(this.btnMeasure_Click);
            // 
            // grpConnection
            // 
            this.grpConnection.Controls.Add(this.cmbDevices);
            this.grpConnection.Controls.Add(this.btnRefresh);
            this.grpConnection.Controls.Add(this.btnConnect);
            this.grpConnection.Location = new System.Drawing.Point(12, 9);
            this.grpConnection.Name = "grpConnection";
            this.grpConnection.Size = new System.Drawing.Size(171, 73);
            this.grpConnection.TabIndex = 4;
            this.grpConnection.TabStop = false;
            this.grpConnection.Text = "Connection";
            // 
            // grpDevice
            // 
            this.grpDevice.Controls.Add(this.label1);
            this.grpDevice.Controls.Add(this.tbDeviceStatus);
            this.grpDevice.Controls.Add(this.lblCurrentRange);
            this.grpDevice.Controls.Add(this.lblCurrent);
            this.grpDevice.Controls.Add(this.tbCurrent);
            this.grpDevice.Controls.Add(this.lblVolt);
            this.grpDevice.Controls.Add(this.tbPotential);
            this.grpDevice.Controls.Add(this.lblPotential);
            this.grpDevice.Location = new System.Drawing.Point(190, 9);
            this.grpDevice.Name = "grpDevice";
            this.grpDevice.Size = new System.Drawing.Size(309, 73);
            this.grpDevice.TabIndex = 5;
            this.grpDevice.TabStop = false;
            this.grpDevice.Text = "设备状态";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(189, 20);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(35, 12);
            this.label1.TabIndex = 7;
            this.label1.Text = "状态:";
            // 
            // tbDeviceStatus
            // 
            this.tbDeviceStatus.Location = new System.Drawing.Point(192, 42);
            this.tbDeviceStatus.Name = "tbDeviceStatus";
            this.tbDeviceStatus.ReadOnly = true;
            this.tbDeviceStatus.Size = new System.Drawing.Size(98, 21);
            this.tbDeviceStatus.TabIndex = 6;
            // 
            // lblCurrentRange
            // 
            this.lblCurrentRange.AutoSize = true;
            this.lblCurrentRange.Location = new System.Drawing.Point(140, 44);
            this.lblCurrentRange.Name = "lblCurrentRange";
            this.lblCurrentRange.Size = new System.Drawing.Size(47, 12);
            this.lblCurrentRange.TabIndex = 5;
            this.lblCurrentRange.Text = "* 10 mA";
            // 
            // lblCurrent
            // 
            this.lblCurrent.AutoSize = true;
            this.lblCurrent.Location = new System.Drawing.Point(7, 44);
            this.lblCurrent.Name = "lblCurrent";
            this.lblCurrent.Size = new System.Drawing.Size(35, 12);
            this.lblCurrent.TabIndex = 4;
            this.lblCurrent.Text = "电流:";
            // 
            // tbCurrent
            // 
            this.tbCurrent.Location = new System.Drawing.Point(64, 42);
            this.tbCurrent.Name = "tbCurrent";
            this.tbCurrent.ReadOnly = true;
            this.tbCurrent.Size = new System.Drawing.Size(70, 21);
            this.tbCurrent.TabIndex = 3;
            // 
            // lblVolt
            // 
            this.lblVolt.AutoSize = true;
            this.lblVolt.Location = new System.Drawing.Point(140, 20);
            this.lblVolt.Name = "lblVolt";
            this.lblVolt.Size = new System.Drawing.Size(11, 12);
            this.lblVolt.TabIndex = 2;
            this.lblVolt.Text = "V";
            // 
            // tbPotential
            // 
            this.tbPotential.Location = new System.Drawing.Point(64, 18);
            this.tbPotential.Name = "tbPotential";
            this.tbPotential.ReadOnly = true;
            this.tbPotential.Size = new System.Drawing.Size(70, 21);
            this.tbPotential.TabIndex = 1;
            // 
            // lblPotential
            // 
            this.lblPotential.AutoSize = true;
            this.lblPotential.Location = new System.Drawing.Point(7, 20);
            this.lblPotential.Name = "lblPotential";
            this.lblPotential.Size = new System.Drawing.Size(35, 12);
            this.lblPotential.TabIndex = 0;
            this.lblPotential.Text = "电位:";
            // 
            // grpMeasurement
            // 
            this.grpMeasurement.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.grpMeasurement.Controls.Add(this.btNyquistZoomin);
            this.grpMeasurement.Controls.Add(this.plotBode);
            this.grpMeasurement.Location = new System.Drawing.Point(1287, 92);
            this.grpMeasurement.Name = "grpMeasurement";
            this.grpMeasurement.Size = new System.Drawing.Size(135, 408);
            this.grpMeasurement.TabIndex = 6;
            this.grpMeasurement.TabStop = false;
            this.grpMeasurement.Text = "绘制Bode曲线";
            // 
            // btNyquistZoomin
            // 
            this.btNyquistZoomin.Location = new System.Drawing.Point(7, 18);
            this.btNyquistZoomin.Name = "btNyquistZoomin";
            this.btNyquistZoomin.Size = new System.Drawing.Size(75, 21);
            this.btNyquistZoomin.TabIndex = 7;
            this.btNyquistZoomin.Text = "放大查看";
            this.btNyquistZoomin.UseVisualStyleBackColor = true;
            // 
            // plotBode
            // 
            this.plotBode.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.plotBode.AutoSize = true;
            this.plotBode.BackColor = System.Drawing.Color.White;
            this.plotBode.Location = new System.Drawing.Point(7, 45);
            this.plotBode.MarkerSize = 5;
            this.plotBode.MarkerType = OxyPlot.MarkerType.Circle;
            this.plotBode.Name = "plotBode";
            this.plotBode.Size = new System.Drawing.Size(122, 357);
            this.plotBode.TabIndex = 4;
            this.plotBode.Title = "Bode";
            this.plotBode.XAxisLabel = null;
            this.plotBode.XAxisType = SDKPlot.AxisType.Linear;
            this.plotBode.YAxisLabel = null;
            this.plotBode.YAxisSecondaryLabel = null;
            this.plotBode.YAxisSecondaryType = SDKPlot.AxisType.Linear;
            this.plotBode.YAxisType = SDKPlot.AxisType.Linear;
            // 
            // grpConsole
            // 
            this.grpConsole.Controls.Add(this.lbConsole);
            this.grpConsole.Location = new System.Drawing.Point(12, 89);
            this.grpConsole.Name = "grpConsole";
            this.grpConsole.Size = new System.Drawing.Size(172, 275);
            this.grpConsole.TabIndex = 7;
            this.grpConsole.TabStop = false;
            this.grpConsole.Text = "消息打印";
            // 
            // lbConsole
            // 
            this.lbConsole.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.lbConsole.FormattingEnabled = true;
            this.lbConsole.ItemHeight = 12;
            this.lbConsole.Location = new System.Drawing.Point(7, 18);
            this.lbConsole.Name = "lbConsole";
            this.lbConsole.Size = new System.Drawing.Size(154, 232);
            this.lbConsole.TabIndex = 0;
            // 
            // psCommSimpleWinForms
            // 
            /*this.psCommSimpleWinForms.EnableBluetooth = false;
            this.psCommSimpleWinForms.EnableSerialPort = false;
            this.psCommSimpleWinForms.Parent = this;
            this.psCommSimpleWinForms.ReceiveStatus += new PalmSens.Comm.StatusEventHandler(this.psCommSimpleWinForms_ReceiveStatus);
            this.psCommSimpleWinForms.MeasurementStarted += new System.EventHandler(this.psCommSimpleWinForms_MeasurementStarted);
            this.psCommSimpleWinForms.MeasurementEnded += new System.EventHandler<System.Exception>(this.psCommSimpleWinForms_MeasurementEnded);
            this.psCommSimpleWinForms.SimpleCurveStartReceivingData += new PalmSens.Core.Simplified.PSCommSimple.SimpleCurveStartReceivingDataHandler(this.psCommSimpleWinForms_SimpleCurveStartReceivingData);
            this.psCommSimpleWinForms.StateChanged += new PalmSens.Comm.CommManager.StatusChangedEventHandler(this.psCommSimpleWinForms_StateChanged);
            this.psCommSimpleWinForms.Disconnected += new PalmSens.Core.Simplified.DisconnectedEventHandler(this.psCommSimpleWinForms_Disconnected);*/
            // 
            // grpFit
            // 
            this.grpFit.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.grpFit.Controls.Add(this.lblIterations);
            this.grpFit.Controls.Add(this.progressBar);
            this.grpFit.Controls.Add(this.lbResults);
            this.grpFit.Controls.Add(this.btnCancel);
            this.grpFit.Controls.Add(this.btnFit);
            this.grpFit.Location = new System.Drawing.Point(12, 370);
            this.grpFit.Name = "grpFit";
            this.grpFit.Size = new System.Drawing.Size(169, 130);
            this.grpFit.TabIndex = 8;
            this.grpFit.TabStop = false;
            this.grpFit.Text = "等效电路拟合";
            // 
            // lblIterations
            // 
            this.lblIterations.AutoSize = true;
            this.lblIterations.Location = new System.Drawing.Point(83, 50);
            this.lblIterations.Name = "lblIterations";
            this.lblIterations.Size = new System.Drawing.Size(0, 12);
            this.lblIterations.TabIndex = 9;
            this.lblIterations.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // progressBar
            // 
            this.progressBar.Location = new System.Drawing.Point(7, 45);
            this.progressBar.MarqueeAnimationSpeed = 0;
            this.progressBar.Name = "progressBar";
            this.progressBar.Size = new System.Drawing.Size(155, 21);
            this.progressBar.Style = System.Windows.Forms.ProgressBarStyle.Marquee;
            this.progressBar.TabIndex = 8;
            // 
            // lbResults
            // 
            this.lbResults.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.lbResults.FormattingEnabled = true;
            this.lbResults.ItemHeight = 12;
            this.lbResults.Location = new System.Drawing.Point(7, 81);
            this.lbResults.Name = "lbResults";
            this.lbResults.Size = new System.Drawing.Size(156, 28);
            this.lbResults.TabIndex = 7;
            // 
            // btnCancel
            // 
            this.btnCancel.Enabled = false;
            this.btnCancel.Location = new System.Drawing.Point(87, 18);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(75, 21);
            this.btnCancel.TabIndex = 6;
            this.btnCancel.Text = "取消拟合";
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // btnFit
            // 
            this.btnFit.Enabled = false;
            this.btnFit.Location = new System.Drawing.Point(6, 18);
            this.btnFit.Name = "btnFit";
            this.btnFit.Size = new System.Drawing.Size(75, 21);
            this.btnFit.TabIndex = 5;
            this.btnFit.Text = "拟合";
            this.btnFit.UseVisualStyleBackColor = true;
            this.btnFit.Click += new System.EventHandler(this.btnFit_Click);
            // 
            // grpParamSet
            // 
            this.grpParamSet.Controls.Add(this.label11);
            this.grpParamSet.Controls.Add(this.tbFaxFre);
            this.grpParamSet.Controls.Add(this.labelFaxFre);
            this.grpParamSet.Controls.Add(this.cmbFreType);
            this.grpParamSet.Controls.Add(this.labelFreType);
            this.grpParamSet.Controls.Add(this.tbStepFre);
            this.grpParamSet.Controls.Add(this.label8);
            this.grpParamSet.Controls.Add(this.label2);
            this.grpParamSet.Controls.Add(this.tbNumFre);
            this.grpParamSet.Controls.Add(this.labelNumFre);
            this.grpParamSet.Controls.Add(this.label9);
            this.grpParamSet.Controls.Add(this.tbEquilibT);
            this.grpParamSet.Controls.Add(this.labelEquilib);
            this.grpParamSet.Controls.Add(this.label7);
            this.grpParamSet.Controls.Add(this.label6);
            this.grpParamSet.Controls.Add(this.tbMinFre);
            this.grpParamSet.Controls.Add(this.tbMaxFre);
            this.grpParamSet.Controls.Add(this.labelMinFre);
            this.grpParamSet.Controls.Add(this.labelMaxFre);
            this.grpParamSet.Controls.Add(this.label3);
            this.grpParamSet.Controls.Add(this.labelAC);
            this.grpParamSet.Controls.Add(this.tbAC);
            this.grpParamSet.Controls.Add(this.label5);
            this.grpParamSet.Controls.Add(this.tbDC);
            this.grpParamSet.Controls.Add(this.labelDC);
            this.grpParamSet.Location = new System.Drawing.Point(516, 10);
            this.grpParamSet.Name = "grpParamSet";
            this.grpParamSet.Size = new System.Drawing.Size(693, 73);
            this.grpParamSet.TabIndex = 9;
            this.grpParamSet.TabStop = false;
            this.grpParamSet.Text = "EIS参数设置";
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(670, 48);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(17, 12);
            this.label11.TabIndex = 25;
            this.label11.Text = "Hz";
            this.label11.Click += new System.EventHandler(this.label11_Click);
            // 
            // tbFaxFre
            // 
            this.tbFaxFre.Location = new System.Drawing.Point(595, 42);
            this.tbFaxFre.Name = "tbFaxFre";
            this.tbFaxFre.Size = new System.Drawing.Size(70, 21);
            this.tbFaxFre.TabIndex = 24;
            this.tbFaxFre.TextChanged += new System.EventHandler(this.textBox3_TextChanged);
            // 
            // labelFaxFre
            // 
            this.labelFaxFre.AutoSize = true;
            this.labelFaxFre.Location = new System.Drawing.Point(529, 48);
            this.labelFaxFre.Name = "labelFaxFre";
            this.labelFaxFre.Size = new System.Drawing.Size(59, 12);
            this.labelFaxFre.TabIndex = 23;
            this.labelFaxFre.Text = "定点频率:";
            this.labelFaxFre.Click += new System.EventHandler(this.label12_Click);
            // 
            // cmbFreType
            // 
            this.cmbFreType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbFreType.FormattingEnabled = true;
            this.cmbFreType.Location = new System.Drawing.Point(595, 17);
            this.cmbFreType.Name = "cmbFreType";
            this.cmbFreType.Size = new System.Drawing.Size(67, 20);
            this.cmbFreType.TabIndex = 3;
            // 
            // labelFreType
            // 
            this.labelFreType.AutoSize = true;
            this.labelFreType.Location = new System.Drawing.Point(529, 22);
            this.labelFreType.Name = "labelFreType";
            this.labelFreType.Size = new System.Drawing.Size(65, 12);
            this.labelFreType.TabIndex = 22;
            this.labelFreType.Text = "扫频/定频:";
            // 
            // tbStepFre
            // 
            this.tbStepFre.Location = new System.Drawing.Point(450, 42);
            this.tbStepFre.Name = "tbStepFre";
            this.tbStepFre.Size = new System.Drawing.Size(35, 21);
            this.tbStepFre.TabIndex = 21;
            this.tbStepFre.TextChanged += new System.EventHandler(this.tbStepFre_TextChanged);
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(433, 45);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(11, 12);
            this.label8.TabIndex = 20;
            this.label8.Text = "/";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(491, 48);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(23, 12);
            this.label2.TabIndex = 19;
            this.label2.Text = "dec";
            this.label2.Click += new System.EventHandler(this.label2_Click);
            // 
            // tbNumFre
            // 
            this.tbNumFre.Location = new System.Drawing.Point(392, 41);
            this.tbNumFre.Name = "tbNumFre";
            this.tbNumFre.Size = new System.Drawing.Size(35, 21);
            this.tbNumFre.TabIndex = 18;
            this.tbNumFre.TextChanged += new System.EventHandler(this.tbNumFre_TextChanged);
            // 
            // labelNumFre
            // 
            this.labelNumFre.AutoSize = true;
            this.labelNumFre.Location = new System.Drawing.Point(327, 44);
            this.labelNumFre.Name = "labelNumFre";
            this.labelNumFre.Size = new System.Drawing.Size(59, 12);
            this.labelNumFre.TabIndex = 17;
            this.labelNumFre.Text = "频率点数:";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(468, 22);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(11, 12);
            this.label9.TabIndex = 16;
            this.label9.Text = "s";
            // 
            // tbEquilibT
            // 
            this.tbEquilibT.Location = new System.Drawing.Point(392, 18);
            this.tbEquilibT.Name = "tbEquilibT";
            this.tbEquilibT.Size = new System.Drawing.Size(70, 21);
            this.tbEquilibT.TabIndex = 15;
            this.tbEquilibT.TextChanged += new System.EventHandler(this.tbEquilibT_TextChanged);
            // 
            // labelEquilib
            // 
            this.labelEquilib.AutoSize = true;
            this.labelEquilib.Location = new System.Drawing.Point(327, 21);
            this.labelEquilib.Name = "labelEquilib";
            this.labelEquilib.Size = new System.Drawing.Size(59, 12);
            this.labelEquilib.TabIndex = 14;
            this.labelEquilib.Text = "静息时间:";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(304, 45);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(17, 12);
            this.label7.TabIndex = 13;
            this.label7.Text = "Hz";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(304, 21);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(17, 12);
            this.label6.TabIndex = 12;
            this.label6.Text = "Hz";
            // 
            // tbMinFre
            // 
            this.tbMinFre.Location = new System.Drawing.Point(228, 42);
            this.tbMinFre.Name = "tbMinFre";
            this.tbMinFre.Size = new System.Drawing.Size(70, 21);
            this.tbMinFre.TabIndex = 11;
            this.tbMinFre.TextChanged += new System.EventHandler(this.tbMinFre_TextChanged);
            // 
            // tbMaxFre
            // 
            this.tbMaxFre.Location = new System.Drawing.Point(228, 18);
            this.tbMaxFre.Name = "tbMaxFre";
            this.tbMaxFre.Size = new System.Drawing.Size(70, 21);
            this.tbMaxFre.TabIndex = 10;
            this.tbMaxFre.TextChanged += new System.EventHandler(this.tbMaxFre_TextChanged);
            // 
            // labelMinFre
            // 
            this.labelMinFre.AutoSize = true;
            this.labelMinFre.Location = new System.Drawing.Point(163, 45);
            this.labelMinFre.Name = "labelMinFre";
            this.labelMinFre.Size = new System.Drawing.Size(59, 12);
            this.labelMinFre.TabIndex = 8;
            this.labelMinFre.Text = "最小频率:";
            // 
            // labelMaxFre
            // 
            this.labelMaxFre.AutoSize = true;
            this.labelMaxFre.Location = new System.Drawing.Point(163, 21);
            this.labelMaxFre.Name = "labelMaxFre";
            this.labelMaxFre.Size = new System.Drawing.Size(59, 12);
            this.labelMaxFre.TabIndex = 7;
            this.labelMaxFre.Text = "最大频率:";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(148, 44);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(11, 12);
            this.label3.TabIndex = 5;
            this.label3.Text = "V";
            // 
            // labelAC
            // 
            this.labelAC.AutoSize = true;
            this.labelAC.Location = new System.Drawing.Point(7, 44);
            this.labelAC.Name = "labelAC";
            this.labelAC.Size = new System.Drawing.Size(59, 12);
            this.labelAC.TabIndex = 4;
            this.labelAC.Text = "交流振幅:";
            // 
            // tbAC
            // 
            this.tbAC.Location = new System.Drawing.Point(72, 41);
            this.tbAC.Name = "tbAC";
            this.tbAC.Size = new System.Drawing.Size(70, 21);
            this.tbAC.TabIndex = 3;
            this.tbAC.TextChanged += new System.EventHandler(this.tbAC_TextChanged);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(146, 20);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(11, 12);
            this.label5.TabIndex = 2;
            this.label5.Text = "V";
            // 
            // tbDC
            // 
            this.tbDC.Location = new System.Drawing.Point(72, 17);
            this.tbDC.Name = "tbDC";
            this.tbDC.Size = new System.Drawing.Size(70, 21);
            this.tbDC.TabIndex = 1;
            this.tbDC.TextChanged += new System.EventHandler(this.tbDC_TextChanged);
            // 
            // labelDC
            // 
            this.labelDC.AutoSize = true;
            this.labelDC.Location = new System.Drawing.Point(7, 20);
            this.labelDC.Name = "labelDC";
            this.labelDC.Size = new System.Drawing.Size(59, 12);
            this.labelDC.TabIndex = 0;
            this.labelDC.Text = "初始电位:";
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.groupBox1.Controls.Add(this.dgvMeasurement);
            this.groupBox1.Controls.Add(this.btnMeasure);
            this.groupBox1.Location = new System.Drawing.Point(190, 92);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(330, 411);
            this.groupBox1.TabIndex = 10;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "测量数据";
            // 
            // dgvMeasurement
            // 
            this.dgvMeasurement.AllowUserToAddRows = false;
            this.dgvMeasurement.AllowUserToDeleteRows = false;
            this.dgvMeasurement.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.dgvMeasurement.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dgvMeasurement.EditMode = System.Windows.Forms.DataGridViewEditMode.EditProgrammatically;
            this.dgvMeasurement.Location = new System.Drawing.Point(9, 48);
            this.dgvMeasurement.Name = "dgvMeasurement";
            this.dgvMeasurement.ReadOnly = true;
            this.dgvMeasurement.RowHeadersVisible = false;
            this.dgvMeasurement.Size = new System.Drawing.Size(315, 357);
            this.dgvMeasurement.TabIndex = 5;
            // 
            // groupBox2
            // 
            this.groupBox2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.groupBox2.Controls.Add(this.btBodeZoomin);
            this.groupBox2.Controls.Add(this.plotNyquist);
            this.groupBox2.Location = new System.Drawing.Point(526, 92);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(755, 408);
            this.groupBox2.TabIndex = 11;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "绘制Nyquist曲线";
            // 
            // btBodeZoomin
            // 
            this.btBodeZoomin.Location = new System.Drawing.Point(7, 17);
            this.btBodeZoomin.Name = "btBodeZoomin";
            this.btBodeZoomin.Size = new System.Drawing.Size(75, 21);
            this.btBodeZoomin.TabIndex = 6;
            this.btBodeZoomin.Text = "放大查看";
            this.btBodeZoomin.UseVisualStyleBackColor = true;
            this.btBodeZoomin.Click += new System.EventHandler(this.btBodeZoomin_Click);
            // 
            // plotNyquist
            // 
            this.plotNyquist.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.plotNyquist.AutoSize = true;
            this.plotNyquist.BackColor = System.Drawing.Color.White;
            this.plotNyquist.Location = new System.Drawing.Point(7, 45);
            this.plotNyquist.MarkerSize = 5;
            this.plotNyquist.MarkerType = OxyPlot.MarkerType.Circle;
            this.plotNyquist.Name = "plotNyquist";
            this.plotNyquist.Size = new System.Drawing.Size(742, 357);
            this.plotNyquist.TabIndex = 4;
            this.plotNyquist.Title = "Nyquist";
            this.plotNyquist.XAxisLabel = null;
            this.plotNyquist.XAxisType = SDKPlot.AxisType.Linear;
            this.plotNyquist.YAxisLabel = null;
            this.plotNyquist.YAxisSecondaryLabel = null;
            this.plotNyquist.YAxisSecondaryType = SDKPlot.AxisType.Linear;
            this.plotNyquist.YAxisType = SDKPlot.AxisType.Linear;
            // 
            // PSSDKPlotEISFitAsyncExample
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1425, 508);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.grpParamSet);
            this.Controls.Add(this.grpFit);
            this.Controls.Add(this.grpConsole);
            this.Controls.Add(this.grpMeasurement);
            this.Controls.Add(this.grpDevice);
            this.Controls.Add(this.grpConnection);
            this.Name = "PSSDKPlotEISFitAsyncExample";
            this.Text = "电化学阻抗谱测量技术（EIS）";
            this.grpConnection.ResumeLayout(false);
            this.grpDevice.ResumeLayout(false);
            this.grpDevice.PerformLayout();
            this.grpMeasurement.ResumeLayout(false);
            this.grpMeasurement.PerformLayout();
            this.grpConsole.ResumeLayout(false);
            this.grpFit.ResumeLayout(false);
            this.grpFit.PerformLayout();
            this.grpParamSet.ResumeLayout(false);
            this.grpParamSet.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dgvMeasurement)).EndInit();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.ResumeLayout(false);

        }

        // 
        // psCommSimpleWinForms
        // 
        /*this.psCommSimpleWinForms.EnableBluetooth = false;
        this.psCommSimpleWinForms.EnableSerialPort = false;
        this.psCommSimpleWinForms.Parent = this;
        this.psCommSimpleWinForms.ReceiveStatus += new PalmSens.Comm.StatusEventHandler(this.psCommSimpleWinForms_ReceiveStatus);
        this.psCommSimpleWinForms.MeasurementStarted += new System.EventHandler(this.psCommSimpleWinForms_MeasurementStarted);
        this.psCommSimpleWinForms.MeasurementEnded += new System.EventHandler<System.Exception>(this.psCommSimpleWinForms_MeasurementEnded);
        this.psCommSimpleWinForms.StateChanged += new PalmSens.Comm.CommManager.StatusChangedEventHandler(this.psCommSimpleWinForms_StateChanged);
        this.psCommSimpleWinForms.Disconnected += new PalmSens.Core.Simplified.DisconnectedEventHandler(this.psCommSimpleWinForms_Disconnected);
        PSSDKBasicAsyncExample.PSSDKBasicAsyncExample1.psCommSimpleWinForms.SimpleCurveStartReceivingData += new PalmSens.Core.Simplified.PSCommSimple.SimpleCurveStartReceivingDataHandler(this.psCommSimpleWinForms_SimpleCurveStartReceivingData);
        */
        #endregion

        private float equilibT = 5f;
        private float dcV = 0f;
        private float acV = 0.005f;
        private float maxFre = 100000f;
        private float minFre = 10f;
        private int   numFre = 72;
        private float stepFre = 17.8f;
        private float faxFre = 100000f;


        //private PalmSens.Core.Simplified.WinForms.PSCommSimpleWinForms psCommSimpleWinForms;
        private System.Windows.Forms.Button btnMeasure;
        private System.Windows.Forms.Button btnConnect;
        private System.Windows.Forms.Button btnRefresh;
        private System.Windows.Forms.ComboBox cmbDevices;
        private System.Windows.Forms.GroupBox grpConnection;
        private System.Windows.Forms.GroupBox grpDevice;
        private System.Windows.Forms.GroupBox grpMeasurement;
        private System.Windows.Forms.GroupBox grpConsole;
        private System.Windows.Forms.ListBox lbConsole;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox tbDeviceStatus;
        private System.Windows.Forms.Label lblCurrentRange;
        private System.Windows.Forms.Label lblCurrent;
        private System.Windows.Forms.TextBox tbCurrent;
        private System.Windows.Forms.Label lblVolt;
        private System.Windows.Forms.TextBox tbPotential;
        private System.Windows.Forms.Label lblPotential;
        private SDKPlot.WinForms.Plot plotBode;
        private System.Windows.Forms.GroupBox grpFit;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.Button btnFit;
        private System.Windows.Forms.Label lblIterations;
        private System.Windows.Forms.ProgressBar progressBar;
        private System.Windows.Forms.ListBox lbResults;
        private System.Windows.Forms.GroupBox grpParamSet;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.TextBox tbEquilibT;
        private System.Windows.Forms.Label labelEquilib;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox tbMinFre;
        private System.Windows.Forms.TextBox tbMaxFre;
        private System.Windows.Forms.Label labelMinFre;
        private System.Windows.Forms.Label labelMaxFre;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label labelAC;
        private System.Windows.Forms.TextBox tbAC;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox tbDC;
        private System.Windows.Forms.Label labelDC;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox tbNumFre;
        private System.Windows.Forms.Label labelNumFre;
        private System.Windows.Forms.TextBox tbStepFre;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.TextBox tbFaxFre;
        private System.Windows.Forms.Label labelFaxFre;
        private System.Windows.Forms.ComboBox cmbFreType;
        private System.Windows.Forms.Label labelFreType;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.DataGridView dgvMeasurement;
        private System.Windows.Forms.GroupBox groupBox2;
        private SDKPlot.WinForms.Plot plotNyquist;
        private System.Windows.Forms.Button btNyquistZoomin;
        private System.Windows.Forms.Button btBodeZoomin;
    }
}

