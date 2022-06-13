public class Script
{
	// ** Wizard Insert Location - Do Not Remove 'Begin/End Wizard Added Module Level Variables' Comments! **
	// Begin Wizard Added Module Level Variables **

	// End Wizard Added Module Level Variables **

	// Add Custom Module Level Variables Here **

	public void InitializeCustomCode()
	{
		// ** Wizard Insert Location - Do not delete 'Begin/End Wizard Added Variable Initialization' lines **
		// Begin Wizard Added Variable Initialization

		//this.JobHead_Column.ColumnChanged += new DataColumnChangeEventHandler(this.JobHead_AfterFieldChange);
		// End Wizard Added Variable Initialization

		// Begin Wizard Added Custom Method Calls

		this.CheckBackflush_Btn.Click += new System.EventHandler(this.CheckBackflush_Btn_Click);
		this.OpenFolder_Btn.Click += new System.EventHandler(this.OpenFolder_Btn_Click);
		// End Wizard Added Custom Method Calls
	}

	public void DestroyCustomCode()
	{
		// ** Wizard Insert Location - Do not delete 'Begin/End Wizard Added Object Disposal' lines **
		// Begin Wizard Added Object Disposal

		this.CheckBackflush_Btn.Click -= new System.EventHandler(this.CheckBackflush_Btn_Click);
		//this.JobHead_Column.ColumnChanged -= new DataColumnChangeEventHandler(this.JobHead_AfterFieldChange);
		this.OpenFolder_Btn.Click -= new System.EventHandler(this.OpenFolder_Btn_Click);
		// End Wizard Added Object Disposal

		// Begin Custom Code Disposal

		// End Custom Code Disposal
	}

	private void CheckBackflush_Btn_Click(object sender, System.EventArgs args)
	{
		// ** Place Event Handling Code Here **

		
		checkStatus backFlush1 = backFlush();
		checkStatus finalOperation1 = finalOperation();
		checkStatus inactiveParts1 = inactiveParts();
		
		string backFlushTest = "PASS";	
		if (backFlush1.error) backFlushTest = "FAIL";
		string finalOperationTest = "PASS";	
		if (finalOperation1.error) finalOperationTest = "FAIL";
		string inactivePartsTest = "PASS";	
		if (inactiveParts1.error) inactivePartsTest = "FAIL";

		string outputMessage = "";

		outputMessage +="--------Parts Missing Backflush--------" + backFlushTest + Environment.NewLine + Environment.NewLine;
		outputMessage += backFlush1.returnMessage + Environment.NewLine + Environment.NewLine;
		//outputMessage +="Error: " + (backFlush1.error).ToString() + Environment.NewLine+ Environment.NewLine;

		outputMessage +="------------Final Operation------------" + finalOperationTest + Environment.NewLine + Environment.NewLine;
		outputMessage += finalOperation1.returnMessage + Environment.NewLine + Environment.NewLine; 
		//outputMessage +="Error: " + (finalOperation1.error).ToString() + Environment.NewLine + Environment.NewLine;

		outputMessage +="------------Inactive Parts-------------" + inactivePartsTest + Environment.NewLine + Environment.NewLine;
		outputMessage += inactiveParts1.returnMessage + Environment.NewLine + Environment.NewLine; 
		//outputMessage +="Error: " + (inactiveParts1.error).ToString() + Environment.NewLine + Environment.NewLine;

		MessageBox.Show(outputMessage);

	}

	private void JobHead_AfterFieldChange(object sender, DataColumnChangeEventArgs args)
	{
		// ** Argument Properties and Uses **
		// args.Row["FieldName"]
		// args.Column, args.ProposedValue, args.Row
		// Add Event Handler Code
		switch (args.Column.ColumnName)
		{
			case "JobReleased":
					if((bool)args.Row["JobReleased"])
					{
					
						checkStatus backFlush1 = backFlush();
						checkStatus finalOperation1 = finalOperation();
						checkStatus inactiveParts1 = inactiveParts();

						string backFlushTest = "PASS";	
						if (backFlush1.error) backFlushTest = "FAIL";
						string finalOperationTest = "PASS";	
						if (finalOperation1.error) finalOperationTest = "FAIL";
						string inactivePartsTest = "PASS";	
						if (inactiveParts1.error) inactivePartsTest = "FAIL";

						if((bool)backFlush1.error || (bool)finalOperation1.error)
	
						{
							string outputMessage = "";
		
							outputMessage +="--------Parts Missing Backflush--------" + backFlushTest + Environment.NewLine + Environment.NewLine;
							outputMessage += backFlush1.returnMessage + Environment.NewLine + Environment.NewLine;
							//outputMessage +="Error: " + (backFlush1.error).ToString() + Environment.NewLine+ Environment.NewLine;
					
							outputMessage +="------------Final Operation------------" + finalOperationTest + Environment.NewLine + Environment.NewLine;
							outputMessage += finalOperation1.returnMessage + Environment.NewLine + Environment.NewLine; 
							//outputMessage +="Error: " + (finalOperation1.error).ToString() + Environment.NewLine + Environment.NewLine;
					
							outputMessage +="------------Inactive Parts-------------" + inactivePartsTest + Environment.NewLine + Environment.NewLine;
							outputMessage += inactiveParts1.returnMessage + Environment.NewLine + Environment.NewLine; 
							//outputMessage +="Error: " + (inactiveParts1.error).ToString() + Environment.NewLine + Environment.NewLine;
					
							MessageBox.Show(outputMessage);
						}
					}
				break;
		}
	}

	//Struct containing two variables a return message and and a true or false value if the status passed
	struct checkStatus
	{
		public string returnMessage;
		public bool error;
	}

	//====================================
	//Check If All Material is Backflushed
	//====================================
	private checkStatus backFlush()
	{
		checkStatus backFlushValues;
		backFlushValues.returnMessage = "";
		backFlushValues.error = false;
		

		EpiDataView JobMtl = ((EpiDataView)oTrans.EpiDataViews["JobMtl"]);

		//Loop though each material sequence and if backflush is off then record the part number
		foreach(DataRow dr in JobMtl.dataView.Table.Rows)
		{
			if(!(bool)dr["Backflush"])
			{
				//MessageBox.Show(dr["Backflush"].ToString());

				backFlushValues.returnMessage += "     " + dr["MtlSeq"].ToString() + "-" + dr["PartNum"].ToString();
				if((bool)dr["dspBuyIt"])
				{
					backFlushValues.returnMessage += "  **On a PO**";
				}
				else
				{
					backFlushValues.returnMessage += "  **SET BACKFLUSH**";
					backFlushValues.error = true;
				}				
				backFlushValues.returnMessage += Environment.NewLine;
			}
		}		

		return backFlushValues;
	}

	//=======================================
	//Check if There is a Final Operation Set
	//=======================================
	private checkStatus finalOperation()
	{
		checkStatus finalOperationValues;
		finalOperationValues.returnMessage = "";
		finalOperationValues.error = false;


		int finalOpCount = 0;
		int finalOperation = 0;

		EpiDataView JobOper = ((EpiDataView)oTrans.EpiDataViews["JobOper"]);
		
		
		//Loop through each operation and determine if one is set to the final operation.
		foreach(DataRow dr in JobOper.dataView.Table.Rows)
		{
			if((bool)dr["FinalOpr"]) 
			{
				finalOpCount++;
				finalOperationValues.returnMessage += "      Final Operation is set to:" + Environment.NewLine + "      " + (int)dr["OprSeq"] + "-" + (string)dr["OpDesc"];
				finalOperation = (int)dr["OprSeq"];
			}
		}
		
		if(finalOpCount < 1) 
		{
			finalOperationValues.error = true;
			finalOperationValues.returnMessage += "      WARNING: Final Operation is not set." + Environment.NewLine + "   Under the tab, Job Details > Operations > Detail, mark the Final Operation checkbox with a check. Do this only for your last sequential operation.";
		}


		//Checks for Resequence issue and provides the solution message to correct it.  When you resequence operations in Epicor the Job Asmbl database does not update.  It has a link to the old sequence number and will cause a null item error to appear.  It can be corrected by following the directions in this error message.  
		EpiDataView JobAsmbl = ((EpiDataView)oTrans.EpiDataViews["JobAsmbl"]);

		foreach(DataRow dr in JobAsmbl.dataView.Table.Rows)
		{

			if(finalOperation != (int)dr["FinalOpr"])
			{
			finalOperationValues.error = true;
			
			finalOperationValues.returnMessage += Environment.NewLine + Environment.NewLine + "Final Operation According to Epicor JobAsmbl Database is: " + (int)dr["FinalOpr"] + Environment.NewLine + Environment.NewLine + "If this operation number " + (int)dr["FinalOpr"] + " does not exist you need to add a new operation with this number " + (int)dr["FinalOpr"] + " and mark it as final operation, uncheck the final operation, save and delete it. After that, you can select the desired operation as final, save and the null item error should disappear.";
			}
		}


		//Loop through each operation and determine if any are set to (OBSOLETE), Check for LASER and make sure it is set to Time and Expense, Check for Paint Estimated Time
		foreach(DataRow dr in JobOper.dataView.Table.Rows)
		{

			if( ((string)dr["OpCodeOpDesc"]).Substring(0,3) == "(OB" )
			{
				finalOperationValues.returnMessage += Environment.NewLine + Environment.NewLine + "------Obsolete Operation----------FAIL--- Operation " + (int)dr["OprSeq"] + Environment.NewLine + Environment.NewLine;
			}	

			if( ((string)dr["OpCode"]).Substring(0,3) == "LAS" && (string)dr["LaborEntryMethod"] != "T")
			{
				finalOperationValues.returnMessage += Environment.NewLine + Environment.NewLine + "------Laser Operation Not Time and Expense----------FAIL--- Operation " + (int)dr["OprSeq"] + Environment.NewLine + Environment.NewLine;
			}

			if( ((string)dr["OpCode"]).Substring(0,3) == "PAI" && (decimal)dr["ProdStandard"] == 0)
			{
				finalOperationValues.returnMessage += Environment.NewLine + Environment.NewLine + "------Paint Production Standard Cannot be Zero----------FAIL--- Operation " + (int)dr["OprSeq"] + Environment.NewLine + Environment.NewLine;
			}				

		}

		
		
		return finalOperationValues;
	}


	//==============================================
	//Check if There is a Material that is Inactive
	//==============================================
	private checkStatus inactiveParts()
	{
		checkStatus inactivePartsValues;
		inactivePartsValues.returnMessage = "";
		inactivePartsValues.error = false;
	
		EpiDataView JobMtl = ((EpiDataView)oTrans.EpiDataViews["JobMtl"]);

		//Loop though each material sequence and search it in the partadapter.  Determine if the part is inactive. 
		foreach(DataRow dr in JobMtl.dataView.Table.Rows)
		{
			if ((string)dr["PartNum"] != "")
			{
			//Connect to the PartDatabase
			PartAdapter partAdapter = new PartAdapter(this.oTrans);
			partAdapter.BOConnect();
				try 
				{
					if (partAdapter.GetByID((string)dr["PartNum"]))
					{		
						if (partAdapter.PartData.Part.Rows.Count > 0)
						{	
							if(Convert.ToBoolean(partAdapter.PartData.Part.Rows[0]["Inactive"]) == true)
							{
								inactivePartsValues.error = true;
								inactivePartsValues.returnMessage += "      " + (int)dr["MtlSeq"] + "-" + (string)dr["PartNum"] + Environment.NewLine;
							}

							if(Convert.ToBoolean(partAdapter.PartData.Part.Rows[0]["Runout"]) == true)
							{
								inactivePartsValues.error = true;
								inactivePartsValues.returnMessage += "      ***Possible Issue*** ...Part set to Run Out..." + Environment.NewLine;
								inactivePartsValues.returnMessage += "      " + (int)dr["MtlSeq"] + "-" + (string)dr["PartNum"] + Environment.NewLine;
							}
						}
					}
				}
				catch(Exception ex)
				{
				}
			// Cleanup
			partAdapter.Dispose();
			}
		}

		return inactivePartsValues;
	}

	private void OpenFolder_Btn_Click(object sender, System.EventArgs args)
	{
		// ** Place Event Handling Code Here **


		EpiDataView EDV = (EpiDataView)oTrans.EpiDataViews["MakeToOrder"];
		
		if(EDV.Row > -1)
		{
	
			//MessageBox.Show("Customer is " + EDV.dataView[EDV.Row]["CustName"].ToString());
	
			Process.Start(@"P:\Customer\" + (EDV.dataView[EDV.Row]["CustName"].ToString()).Substring(0,1));
	
		}
		else {
			Process.Start(@"P:\Customer\");
		}
		
	}
}
