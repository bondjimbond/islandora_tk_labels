<?php
/**
 * @file
 * TK Labels badge admin options
 */
function islandora_tk_labels_admin_form($form, $form_settings) {
  $form['islandora_tk_labels_project_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Solr field for TK Labels Project ID'),
    '#description' => t('The solr field that contains the TK Labels Project Identifier.'),
    '#default_value' => variable_get('islandora_tk_labels_project_field', 'dc.identifier'),
  );
  $form['islandora_tk_labels_image_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Image Height'),
    '#description' => t('The height in pixels for the TK Labels badge image.'),
    '#default_value' => variable_get('islandora_tk_labels_image_height', '31'),
  );
  $form = system_settings_form($form);
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
