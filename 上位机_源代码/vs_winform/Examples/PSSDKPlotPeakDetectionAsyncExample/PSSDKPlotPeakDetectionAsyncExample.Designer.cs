namespace PSSDKPlotPeakDetectionAsyncExample
{
    partial class PSSDKPlotPeakDetectionAsyncExample
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
            this.btnPeaks = new System.Windows.Forms.Button();
            this.plot = new SDKPlot.WinForms.Plot();
            this.grpConsole = new System.Windows.Forms.GroupBox();
            this.lbConsole = new System.Windows.Forms.ListBox();
            this.grpParamSet = new System.Windows.Forms.GroupBox();
            this.label4 = new System.Windows.Forms.Label();
            this.tbnScan = new System.Windows.Forms.TextBox();
            this.labelnScan = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.tbFinal = new System.Windows.Forms.TextBox();
            this.labelFinal = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.tbStep = new System.Windows.Forms.TextBox();
            this.labelStep = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.tbEquilib = new System.Windows.Forms.TextBox();
            this.labelEquilib = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.tbRate = new System.Windows.Forms.TextBox();
            this.tbLow = new System.Windows.Forms.TextBox();
            this.labelRate = new System.Windows.Forms.Label();
            this.labelLow = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.labelHigh = new System.Windows.Forms.Label();
            this.tbHigh = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.tbBegin = new System.Windows.Forms.TextBox();
            this.labelBegin = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.dgvMeasurement = new System.Windows.Forms.DataGridView();
            this.grpConnection.SuspendLayout();
            this.grpDevice.SuspendLayout();
            this.grpMeasurement.SuspendLayout();
            this.grpConsole.SuspendLayout();
            this.grpParamSet.SuspendLayout();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dgvMeasurement)).BeginInit();
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
            this.btnMeasure.Location = new System.Drawing.Point(6, 20);
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
            this.grpConnection.Text = "设备连接";
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
            this.grpDevice.Size = new System.Drawing.Size(307, 73);
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
            this.tbDeviceStatus.ForeColor = System.Drawing.SystemColors.GrayText;
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
            this.tbCurrent.ForeColor = System.Drawing.SystemColors.GrayText;
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
            this.tbPotential.ForeColor = System.Drawing.SystemColors.GrayText;
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
            this.grpMeasurement.Controls.Add(this.btnPeaks);
            this.grpMeasurement.Controls.Add(this.plot);
            this.grpMeasurement.Location = new System.Drawing.Point(398, 89);
            this.grpMeasurement.Name = "grpMeasurement";
            this.grpMeasurement.Size = new System.Drawing.Size(820, 344);
            this.grpMeasurement.TabIndex = 6;
            this.grpMeasurement.TabStop = false;
            this.grpMeasurement.Text = "曲线绘制";
            // 
            // btnPeaks
            // 
            this.btnPeaks.Enabled = false;
            this.btnPeaks.Location = new System.Drawing.Point(7, 18);
            this.btnPeaks.Name = "btnPeaks";
            this.btnPeaks.Size = new System.Drawing.Size(85, 21);
            this.btnPeaks.TabIndex = 5;
            this.btnPeaks.Text = "峰值检测";
            this.btnPeaks.UseVisualStyleBackColor = true;
            this.btnPeaks.Click += new System.EventHandler(this.btnPeaks_Click);
            // 
            // plot
            // 
            this.plot.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.plot.BackColor = System.Drawing.Color.White;
            this.plot.Location = new System.Drawing.Point(7, 45);
            this.plot.MarkerSize = 5;
            this.plot.MarkerType = OxyPlot.MarkerType.Circle;
            this.plot.Name = "plot";
            this.plot.Size = new System.Drawing.Size(807, 293);
            this.plot.TabIndex = 4;
            this.plot.Title = null;
            this.plot.XAxisLabel = null;
            this.plot.XAxisType = SDKPlot.AxisType.Linear;
            this.plot.YAxisLabel = null;
            this.plot.YAxisSecondaryLabel = null;
            this.plot.YAxisSecondaryType = SDKPlot.AxisType.Linear;
            this.plot.YAxisType = SDKPlot.AxisType.Linear;
            // 
            // grpConsole
            // 
            this.grpConsole.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.grpConsole.Controls.Add(this.lbConsole);
            this.grpConsole.Location = new System.Drawing.Point(12, 89);
            this.grpConsole.Name = "grpConsole";
            this.grpConsole.Size = new System.Drawing.Size(172, 344);
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
            this.lbConsole.Location = new System.Drawing.Point(3, 17);
            this.lbConsole.Name = "lbConsole";
            this.lbConsole.Size = new System.Drawing.Size(166, 316);
            this.lbConsole.TabIndex = 0;
            // 
            // grpParamSet
            // 
            this.grpParamSet.Controls.Add(this.label4);
            this.grpParamSet.Controls.Add(this.tbnScan);
            this.grpParamSet.Controls.Add(this.labelnScan);
            this.grpParamSet.Controls.Add(this.label8);
            this.grpParamSet.Controls.Add(this.tbFinal);
            this.grpParamSet.Controls.Add(this.labelFinal);
            this.grpParamSet.Controls.Add(this.label2);
            this.grpParamSet.Controls.Add(this.tbStep);
            this.grpParamSet.Controls.Add(this.labelStep);
            this.grpParamSet.Controls.Add(this.label9);
            this.grpParamSet.Controls.Add(this.tbEquilib);
            this.grpParamSet.Controls.Add(this.labelEquilib);
            this.grpParamSet.Controls.Add(this.label7);
            this.grpParamSet.Controls.Add(this.label6);
            this.grpParamSet.Controls.Add(this.tbRate);
            this.grpParamSet.Controls.Add(this.tbLow);
            this.grpParamSet.Controls.Add(this.labelRate);
            this.grpParamSet.Controls.Add(this.labelLow);
            this.grpParamSet.Controls.Add(this.label3);
            this.grpParamSet.Controls.Add(this.labelHigh);
            this.grpParamSet.Controls.Add(this.tbHigh);
            this.grpParamSet.Controls.Add(this.label5);
            this.grpParamSet.Controls.Add(this.tbBegin);
            this.grpParamSet.Controls.Add(this.labelBegin);
            this.grpParamSet.Location = new System.Drawing.Point(503, 10);
            this.grpParamSet.Name = "grpParamSet";
            this.grpParamSet.Size = new System.Drawing.Size(709, 73);
            this.grpParamSet.TabIndex = 9;
            this.grpParamSet.TabStop = false;
            this.grpParamSet.Text = "CV参数设置";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(695, 45);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(11, 12);
            this.label4.TabIndex = 25;
            this.label4.Text = "s";
            // 
            // tbnScan
            // 
            this.tbnScan.Location = new System.Drawing.Point(619, 41);
            this.tbnScan.Name = "tbnScan";
            this.tbnScan.Size = new System.Drawing.Size(70, 21);
            this.tbnScan.TabIndex = 24;
            this.tbnScan.TextChanged += new System.EventHandler(this.tbnScan_TextChanged);
            // 
            // labelnScan
            // 
            this.labelnScan.AutoSize = true;
            this.labelnScan.Location = new System.Drawing.Point(554, 44);
            this.labelnScan.Name = "labelnScan";
            this.labelnScan.Size = new System.Drawing.Size(59, 12);
            this.labelnScan.TabIndex = 23;
            this.labelnScan.Text = "扫描圈数:";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(330, 44);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(11, 12);
            this.label8.TabIndex = 22;
            this.label8.Text = "V";
            // 
            // tbFinal
            // 
            this.tbFinal.Enabled = false;
            this.tbFinal.ForeColor = System.Drawing.SystemColors.GrayText;
            this.tbFinal.Location = new System.Drawing.Point(254, 41);
            this.tbFinal.Name = "tbFinal";
            this.tbFinal.Size = new System.Drawing.Size(70, 21);
            this.tbFinal.TabIndex = 21;
            this.tbFinal.TextChanged += new System.EventHandler(this.tbFinal_TextChanged);
            // 
            // labelFinal
            // 
            this.labelFinal.AutoSize = true;
            this.labelFinal.Location = new System.Drawing.Point(189, 44);
            this.labelFinal.Name = "labelFinal";
            this.labelFinal.Size = new System.Drawing.Size(59, 12);
            this.labelFinal.TabIndex = 20;
            this.labelFinal.Text = "终点电位:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(522, 20);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(11, 12);
            this.label2.TabIndex = 19;
            this.label2.Text = "V";
            // 
            // tbStep
            // 
            this.tbStep.Location = new System.Drawing.Point(446, 17);
            this.tbStep.Name = "tbStep";
            this.tbStep.Size = new System.Drawing.Size(70, 21);
            this.tbStep.TabIndex = 18;
            this.tbStep.TextChanged += new System.EventHandler(this.tbStep_TextChanged);
            // 
            // labelStep
            // 
            this.labelStep.AutoSize = true;
            this.labelStep.Location = new System.Drawing.Point(381, 20);
            this.labelStep.Name = "labelStep";
            this.labelStep.Size = new System.Drawing.Size(59, 12);
            this.labelStep.TabIndex = 17;
            this.labelStep.Text = "电位步进:";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(695, 19);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(11, 12);
            this.label9.TabIndex = 16;
            this.label9.Text = "s";
            // 
            // tbEquilib
            // 
            this.tbEquilib.Location = new System.Drawing.Point(619, 15);
            this.tbEquilib.Name = "tbEquilib";
            this.tbEquilib.Size = new System.Drawing.Size(70, 21);
            this.tbEquilib.TabIndex = 15;
            this.tbEquilib.TextChanged += new System.EventHandler(this.tbEquilib_TextChanged);
            // 
            // labelEquilib
            // 
            this.labelEquilib.AutoSize = true;
            this.labelEquilib.Location = new System.Drawing.Point(554, 18);
            this.labelEquilib.Name = "labelEquilib";
            this.labelEquilib.Size = new System.Drawing.Size(59, 12);
            this.labelEquilib.TabIndex = 14;
            this.labelEquilib.Text = "静息时间:";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(522, 43);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(23, 12);
            this.label7.TabIndex = 13;
            this.label7.Text = "V/s";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(330, 20);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(11, 12);
            this.label6.TabIndex = 12;
            this.label6.Text = "V";
            // 
            // tbRate
            // 
            this.tbRate.Location = new System.Drawing.Point(446, 40);
            this.tbRate.Name = "tbRate";
            this.tbRate.Size = new System.Drawing.Size(70, 21);
            this.tbRate.TabIndex = 11;
            this.tbRate.TextChanged += new System.EventHandler(this.tbRate_TextChanged);
            // 
            // tbLow
            // 
            this.tbLow.Location = new System.Drawing.Point(254, 17);
            this.tbLow.Name = "tbLow";
            this.tbLow.Size = new System.Drawing.Size(70, 21);
            this.tbLow.TabIndex = 10;
            this.tbLow.TextChanged += new System.EventHandler(this.tbLow_TextChanged);
            // 
            // labelRate
            // 
            this.labelRate.AutoSize = true;
            this.labelRate.Location = new System.Drawing.Point(381, 43);
            this.labelRate.Name = "labelRate";
            this.labelRate.Size = new System.Drawing.Size(59, 12);
            this.labelRate.TabIndex = 8;
            this.labelRate.Text = "扫描速度:";
            // 
            // labelLow
            // 
            this.labelLow.AutoSize = true;
            this.labelLow.Location = new System.Drawing.Point(189, 20);
            this.labelLow.Name = "labelLow";
            this.labelLow.Size = new System.Drawing.Size(59, 12);
            this.labelLow.TabIndex = 7;
            this.labelLow.Text = "低点电位:";
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
            // labelHigh
            // 
            this.labelHigh.AutoSize = true;
            this.labelHigh.Location = new System.Drawing.Point(7, 44);
            this.labelHigh.Name = "labelHigh";
            this.labelHigh.Size = new System.Drawing.Size(59, 12);
            this.labelHigh.TabIndex = 4;
            this.labelHigh.Text = "高点电位:";
            // 
            // tbHigh
            // 
            this.tbHigh.Location = new System.Drawing.Point(72, 41);
            this.tbHigh.Name = "tbHigh";
            this.tbHigh.Size = new System.Drawing.Size(70, 21);
            this.tbHigh.TabIndex = 3;
            this.tbHigh.TextChanged += new System.EventHandler(this.tbHigh_TextChanged);
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
            // tbBegin
            // 
            this.tbBegin.Location = new System.Drawing.Point(72, 17);
            this.tbBegin.Name = "tbBegin";
            this.tbBegin.Size = new System.Drawing.Size(70, 21);
            this.tbBegin.TabIndex = 1;
            this.tbBegin.TextChanged += new System.EventHandler(this.tbBegin_TextChanged);
            // 
            // labelBegin
            // 
            this.labelBegin.AutoSize = true;
            this.labelBegin.Location = new System.Drawing.Point(7, 20);
            this.labelBegin.Name = "labelBegin";
            this.labelBegin.Size = new System.Drawing.Size(59, 12);
            this.labelBegin.TabIndex = 0;
            this.labelBegin.Text = "起始电位:";
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.groupBox1.Controls.Add(this.dgvMeasurement);
            this.groupBox1.Controls.Add(this.btnMeasure);
            this.groupBox1.Location = new System.Drawing.Point(190, 88);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(202, 345);
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
            this.dgvMeasurement.Location = new System.Drawing.Point(6, 45);
            this.dgvMeasurement.Name = "dgvMeasurement";
            this.dgvMeasurement.ReadOnly = true;
            this.dgvMeasurement.RowHeadersVisible = false;
            this.dgvMeasurement.Size = new System.Drawing.Size(193, 291);
            this.dgvMeasurement.TabIndex = 4;
            // 
            // PSSDKPlotPeakDetectionAsyncExample
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1230, 444);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.grpParamSet);
            this.Controls.Add(this.grpConsole);
            this.Controls.Add(this.grpMeasurement);
            this.Controls.Add(this.grpDevice);
            this.Controls.Add(this.grpConnection);
            this.Name = "PSSDKPlotPeakDetectionAsyncExample";
            this.Text = "循环伏安法测量技术（CV）";
            this.grpConnection.ResumeLayout(false);
            this.grpDevice.ResumeLayout(false);
            this.grpDevice.PerformLayout();
            this.grpMeasurement.ResumeLayout(false);
            this.grpConsole.ResumeLayout(false);
            this.grpParamSet.ResumeLayout(false);
            this.grpParamSet.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dgvMeasurement)).EndInit();
            this.ResumeLayout(false);

        }
           // 
            // psCommSimpleWinForms
            // 
 /*         this.psCommSimpleWinForms.EnableBluetooth = false;
            this.psCommSimpleWinForms.EnableSerialPort = false;
            this.psCommSimpleWinForms.Parent = this;
            this.psCommSimpleWinForms.ReceiveStatus += new PalmSens.Comm.StatusEventHandler(this.psCommSimpleWinForms_ReceiveStatus);
            this.psCommSimpleWinForms.MeasurementStarted += new System.EventHandler(this.psCommSimpleWinForms_MeasurementStarted);
            this.psCommSimpleWinForms.MeasurementEnded += new System.EventHandler<System.Exception>(this.psCommSimpleWinForms_MeasurementEnded);
            this.psCommSimpleWinForms.SimpleCurveStartReceivingData += new PalmSens.Core.Simplified.PSCommSimple.SimpleCurveStartReceivingDataHandler(this.psCommSimpleWinForms_SimpleCurveStartReceivingData);
            this.psCommSimpleWinForms.StateChanged += new PalmSens.Comm.CommManager.StatusChangedEventHandler(this.psCommSimpleWinForms_StateChanged);
            this.psCommSimpleWinForms.Disconnected += new PalmSens.Core.Simplified.DisconnectedEventHandler(this.psCommSimpleWinForms_Disconnected);
        */
        #endregion

        private float beginV = -0.2f;
        private float highV = 0.6f;
        private float lowV = -0.2f;
        private float finalV = -0.2f;
        private float stepV = 0.001f;
        private float rateV = 0.05f;
        private float equilibT = 5f;
        private int nScan = 1;

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
        private SDKPlot.WinForms.Plot plot;
        private System.Windows.Forms.Button btnPeaks;
        private System.Windows.Forms.GroupBox grpParamSet;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.TextBox tbEquilib;
        private System.Windows.Forms.Label labelEquilib;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox tbRate;
        private System.Windows.Forms.TextBox tbLow;
        private System.Windows.Forms.Label labelRate;
        private System.Windows.Forms.Label labelLow;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label labelHigh;
        private System.Windows.Forms.TextBox tbHigh;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox tbBegin;
        private System.Windows.Forms.Label labelBegin;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox tbStep;
        private System.Windows.Forms.Label labelStep;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.TextBox tbFinal;
        private System.Windows.Forms.Label labelFinal;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox tbnScan;
        private System.Windows.Forms.Label labelnScan;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.DataGridView dgvMeasurement;
    }
}

