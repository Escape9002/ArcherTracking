/*
Written by Antonio R.
Sources:
    https://medium.com/@martijn.van.welie/making-android-ble-work-part-1-a736dcd53b02
    Stack overflow
    ...
UUIDs:
   Service Uuid: c54beb4a-40c7-11eb-b378-0242ac130002
   characteristics Uuid [String]: d6b78de4-40c7-11eb-b378-0242ac130002

 */

package com.example.andble11;
import androidx.appcompat.app.AppCompatActivity;
import android.Manifest;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.le.BluetoothLeScanner;
import android.bluetooth.le.ScanCallback;
import android.bluetooth.le.ScanFilter;
import android.bluetooth.le.ScanResult;
import android.bluetooth.le.ScanSettings;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.ListAdapter;
import android.widget.ListView;
import android.widget.TextView;

import java.util.ArrayList;
import java.util.List;


public class MainActivity extends AppCompatActivity {

    //----------------------------------------------------------------------------------------------PERMISSION-CHECK-VARIABLES
    private static final int REQUEST_ENABLE_BT = 1;
    private static final int ACCESS_FINE_LOCATION_REQUEST = 2;
    //----------------------------------------------------------------------------------------------PERMISSION-CHECK-VARIABLES
    //----------------------------------------------------------------------------------------------BLE-CONNECT-VARIABLES
    private static final int TRANSPORT_LE = 64;
    private static final boolean autoconnect = false;
    //----------------------------------------------------------------------------------------------BLE-CONNECT-VARIABLES
    //----------------------------------------------------------------------------------------------TEXTVIEW | BUTTON | LISTS | ETC
    TextView BleStatusTxt = (TextView) findViewById(R.id.status_scan);
    TextView foundDevtxt = (TextView) findViewById(R.id.foundDevTxt);
    //----------------------------------------------------------------------------------------------TEXTVIEW | BUTTON | LISTS | ETC
    //----------------------------------------------------------------------------------------------BLE-SETUP
    //BluetoothAdapter is hardware connection | BluetoothScanner is to scan
    BluetoothAdapter adapter = BluetoothAdapter.getDefaultAdapter();
    BluetoothLeScanner scanner = adapter.getBluetoothLeScanner();

    //TAG for msg | filters to find BLE | scanSettings to filter again | scanCallback to find devices
    static final String TAG = "default"; //set default msg
    static List<ScanFilter> filters = null;         //set empty/ no filters   --> ERROR?
    ScanSettings scanSettings = new ScanSettings.Builder()
            .setScanMode(ScanSettings.SCAN_MODE_LOW_LATENCY)                //long scans,high battery drain, more likely to find devices
            .setCallbackType(ScanSettings.CALLBACK_TYPE_FIRST_MATCH )       //gives callback after first seen, "ignores" afterwards
            .setMatchMode(ScanSettings.MATCH_MODE_AGGRESSIVE)               //every device is seen (even with few advertisements/ low signal strength)
            .setNumOfMatches(ScanSettings.MATCH_NUM_ONE_ADVERTISEMENT)      //one advertisement is needed for trigger
            .setReportDelay(0L)
            .build();

    //----------------------------------------------------------------------------------------------BLE-SETUP
    //----------------------------------------------------------------------------------------------ON-CREATE
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        System.out.print("Starting!");

        if(!adapter.isEnabled()) {
            Intent enableBTIntent = new Intent(adapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBTIntent, REQUEST_ENABLE_BT);
        }
    }
    //----------------------------------------------------------------------------------------------ON-CREATE
    //----------------------------------------------------------------------------------------------BUTTON-DECLARATION
    //-----------------------------------------SEARCH-BUTTON
    public void startScan(View view){
        onSearch();
    }
    //-----------------------------------------SEARCH-BUTTON
    //-----------------------------------------CONNECT-BUTTON
    public void startConnecting(View view){
    }
    //-----------------------------------------CONNECT-BUTTON
    //-----------------------------------------DISCONNECT-BUTTON
    public void startDisconnecting(View view){
    }
    //-----------------------------------------DISCONNECT-BUTTON
    //----------------------------------------------------------------------------------------------BUTTON-DECLARATION
    //----------------------------------------------------------------------------------------------ON-SEARCH
    protected void onSearch(){
        System.out.print("Scanning");

        if (scanner != null) {
            scanner.startScan(filters, scanSettings, scanCallback);
            Log.d(TAG, "scan started");
            BleStatusTxt.setText("Status Scan: Started!");


        }  else {
            Log.e(TAG, "could not get scanner object");
            BleStatusTxt.setText("Status Scan: Failed!");
        }

    }
    //----------------------------------------------------------------------------------------------ON-SEARCH
    //----------------------------------------------------------------------------------------------SCAN CALLBACK
    private final ScanCallback scanCallback = new ScanCallback() {


        @Override
        public void onScanResult(int callbackType, ScanResult result) {
            System.out.print("Callback");
            BluetoothDevice device = result.getDevice();
            // ...do whatever you want with this found device
        foundDevtxt.setText(device.getName());
        }

        @Override
        public void onBatchScanResults(List<ScanResult> results) {
            System.out.print("Callback");
            // Ignore for now
        }

        @Override
        public void onScanFailed(int errorCode) {
            System.out.print("Callback");
            // Ignore for now
        }
    };
    //----------------------------------------------------------------------------------------------SCAN CALLBACK
    //----------------------------------------------------------------------------------------------ON-CONNECT

    /* BluetoothGatt gatt = device.connectGatt(
    // context,                                                                                     --> needed by android to connect
    // autoconnect,                                                                                 --> autoconnect decides wether the device connect immediately or not. FALSE: connect immediately (connecting for 30sec, if failing ERROR_CODE 133) | TRUE: autoconnecting with no timeout (connecting to cached devices).
    // bluetoothGattCallback,                                                                       --> action to read/write/ everything device specific
    // TRANSPORT_LE);                                                                               --> TRANSPORT_LE must be declared by yourself, it's probably the approximate amount of received data
     */

    //----------------------------------------------------------------------------------------------ON-CONNECT
    //----------------------------------------------------------------------------------------------PERMISSION-GRANTED?
    private boolean hasPermissions() {
        System.out.print("PermissionCheck1");
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            if (getApplicationContext().checkSelfPermission(Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
                requestPermissions(new String[] { Manifest.permission.ACCESS_FINE_LOCATION }, ACCESS_FINE_LOCATION_REQUEST);
                return false;
            }
        }

        if (!adapter.isEnabled() ){
            System.out.print("PermissionCheck2");
            Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
        }
        return true;
    }
    //----------------------------------------------------------------------------------------------PERMISSION-GRANTED?
}