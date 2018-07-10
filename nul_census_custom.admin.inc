<?php

function nul_census_custom_settings_form($form, &$form_state) {
  // Creates a settings form for the settings for the module
  civicrm_initialize();
  require_once 'api/api.php';

  $form['civisync'] = array(
    '#markup' => '<p>To Sync data to civi visit <a href="/civisync">CiviCRM Sync</a></p>',
  );
  $form['#submit'] = array('nul_census_custom_settings_form_submit');
  // Email to send notification of status changes to/from
  // TODO make seperate setting for email notifications from and email notifications to
  // TODO also send emails to CEO/Contact email
  $form['emailnotification'] = array(
    '#type' => 'textfield',
    '#title' => t('From Email Address for Census Status update Notifications'),
    '#description' => t('List the emails that should be notified when censuses change statuses separted by commas, in addtion to emails listed here email notifications will be sent to the email addresses on the census'),
    '#default_value' => variable_get('emailnotification'),
  );

  // Field in Civi to use to find the affiliate
  $form['affiliatefield'] = array(
    '#type' => 'textfield',
    '#title' => t('ID of custom field in civi to base affiliate off of'),
    '#default_value' => variable_get('affiliatefield'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save',
  );
  return $form;
}

function nul_census_custom_settings_form_submit($form, &$form_state) {
  $fields = array('affiliatefield', 'emailnotification');
  foreach ($fields as $key => $field) {
    if (!empty($form[$field]['#value'])) {
      variable_set($field, $form[$field]['#value']);
    }
  }
  drupal_set_message(t('The settings have been saved'));
}
