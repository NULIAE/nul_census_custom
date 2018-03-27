<?php

// Print out header row
$headerOverRide = array(
  'content_type' => 'Tab',
  'field' => 'Field',
  'value' => 'Value',
);

print implode($separator, $headerOverRide) . "\r\n";
