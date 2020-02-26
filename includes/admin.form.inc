<?php
/**
 * @file
 * TK Labels badge admin options
 */
function islandora_tk_labels_admin_form($form, $form_settings) {
  $form['islandora_tk_labels_rights_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Solr field for TK Labels URI'),
    '#description' => t('The solr field that contains the TK Labels URI'),
    '#default_value' => variable_get('islandora_tk_labels_rights_field', 'dc.rights'),
  );

  $form['islandora_tk_labels_image_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Image Height'),
    '#description' => t('The height in pixels for the TK Labels badge image.'),
    '#default_value' => variable_get('islandora_tk_labels_image_height', '31'),
  );
  
  $form['islandora_tk_labels_user'] = array(
    '#type' => 'textfield',
    '#title' => t('API user ID'),
    '#description' => t('Your user ID for the TK Labels API.'),
    '#default_value' => variable_get('islandora_tk_labels_user', ''),
  );

  $passexist = variable_get('islandora_tk_labels_password', 0);
  if (isset($passexist)) {
    $password_message = "Password is saved";
  }
  else {
    $password_message = "No password saved";
  }
  $form['islandora_tk_labels_password'] = array(
    '#type' => 'password',
    '#title' => t('API password'),
    '#description' => t('Your password for the TK Labels API. Leave blank if password is already saved.
      <b>Current status: @password_message</b>.', array(
        '@password_message' => $password_message,
      )
    ),
    '#required' => !isset($passexist),
  );
  $form = system_settings_form($form);
  array_unshift($form['#submit'], 'islandora_tk_labels_default_password');
  return $form;
}

function islandora_tk_labels_default_password($form, &$form_state) {
  if ($form_state['values']['islandora_tk_labels_password'] == '') {
    $form_state['values']['islandora_tk_labels_password'] = variable_get('islandora_tk_labels_password', '');
  }
  $form['buttons']['submit'] = array('#type' => 'submit', '#value' => t('Save configuration'));
  $form['#submit'] = array('islandora_tk_labels_password', 'islandora_tk_labels_admin_submit_form');

  return $form;
}

function islandora_tk_labels_submit_form($form, &$form_state) {
    foreach ($form_state['values'] as $key => $value) {
    if (is_array($value) && isset($form_state['values']['array_filter'])) {
      $value = array_keys(array_filter($value));
    }
    variable_set($key, $value);
  }
  drupal_set_message(t('The settings have been updated!'));
}
