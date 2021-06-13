package com.example.andble12;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
//initial Code
public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        System.out.print("Hello World");
    }
}