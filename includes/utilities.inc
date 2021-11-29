<?php

/**
 * @file
 * Utility functions.
 */

/**
 * Take URI, return HTML code for a TK Labels badge.
 * @return string
 */

/**
 * Get URI from the configured Solr field.
 * @return string
 */
function islandora_tk_labels_get_uri(AbstractObject $object) {
  // Gets the TK Labels URI from the configured Solr field.
  $qp = new islandoraSolrQueryProcessor();
  $qp->buildQuery(format_string('@field:"@pid"', array(
    '@field' => 'PID',
    '@pid' =>"{$object->id}")
  ));
  $project_field = variable_get('islandora_tk_labels_project_field', 'dc.identifier');
  $qp->solrParams['fl'] = implode(', ', array(
    'PID',
    $project_field,
  ));
  $qp->solrStart = 0;
  $qp->solrLimit = 100000;
  $qp->executeQuery(FALSE);
  if ($qp->islandoraSolrResult['response']['numFound'] > 0) {
    if (array_key_exists($project_field, $qp->islandoraSolrResult['response']['objects']['0']['solr_doc'])) {
      $identifier_array = ($qp->islandoraSolrResult['response']['objects']['0']['solr_doc'][$project_field]);
      if (is_array($identifier_array)) {
        foreach ($identifier_array as $identifier) {
          $project_id = islandora_tk_labels_find_project_id($identifier);
        }
      }
      else {
        $project_id = islandora_tk_labels_find_project_id($identifier_array);
      }
    }
  }
  if (!isset($project_id)) {
    return;
  }
  else {
    return $project_id;
  }
}

function islandora_tk_labels_find_project_id($identifier) {
  $identifier_parts = explode("-", $identifier);
  if (count($identifier_parts) == 5) {
    $project_id = $identifier;
  } 
  return $project_id;
}

/**
 * Take URI, return HTML code for a rights statement badge.
 * @return string
 */
function islandora_tk_labels_get_html($project_id) {
  $height = variable_get('islandora_tk_labels_image_height', '31');
  // Extract the relevant info from the URI and translate to an API URI.
  $base_url = "https://localcontextshub.org/api/v1/projects/";

  $request_url = $base_url . $project_id . "/";

  $options = array(
    'method' => 'GET',
    'headers' => array(
      'Accept' => 'application/json',
    ),
  );

  // The result has lots of objects that must be turned into arrays.
  $result = get_object_vars(drupal_http_request($request_url, $options));
  // If there is a timeout checking for badges, we exit early.
  if ($result['code'] == "-110" || $result['code'] == "404") {
    return;
  }

  $result = get_object_vars(json_decode($result['data']));
  // In the JSON response, the 'notice' key only exists if there is no label yet applied.
  if (array_key_exists('notice', $result)) {
    $notice = get_object_vars($result['notice'][0]);
  }
  if (array_key_exists('bc_labels', $result)) {
    $bc_labels = $result['bc_labels'];
  }
  if (array_key_exists('tk_labels', $result)) {
    $tk_labels = $result['tk_labels'];
  }

  $project_title = $result['title'];

  if (isset($notice)) {
    $notice_type = $notice['notice_type'];
    $bc_image_url = $notice['bc_img_url'];
    $bc_text = $notice['bc_default_text'];
    $tk_image_url = $notice['tk_img_url'];
    $tk_text = $notice['tk_default_text'];
    $title = $notice_type;
    $bc_image = array(
      '#theme' => 'image',
      '#path' => $bc_image_url,
      '#alt' => $bc_text,
      '#height' => $height,
      '#title' => 'Biocultural Notice',
    );
    $bc_markup = '<div class="tk_icon">' . drupal_render($bc_image) . '</div><div class="lc_textblock" style="display:none"><div class="tk_text">' . $bc_text . '</div></div>';
    $bc_badge = array(
      '#type' => 'markup',
      '#title' => 'Biocultural Notice',
      '#markup' => $bc_markup,
    );
    $tk_image = array(
      '#theme' => 'image',
      '#path' => $tk_image_url,
      '#alt' => $tk_text,
      '#height' => $height,
      '#title' => "Traditional Knowledge Label",
    );
    $tk_markup = '<div class="tk_icon">' . drupal_render($tk_image) . '</div><div class="lc_textblock" style="display:none"><div class="tk_text">' . $tk_text . '</div></div>';
    $tk_badge = array(
      '#type' => 'markup',
      '#title' => 'Traditional Knowledge Label',
      '#markup' => $tk_markup,                                                                                              
    );

    $badge = array(
      $bc_badge,
      $tk_badge,
    );
  }
  $badge_array = array();
  if (isset($bc_labels)) {
    foreach($bc_labels as $bc_label) {
      $bc_label = get_object_vars($bc_label);
      $label_type = $bc_label['name'];
      $bc_image_url = $bc_label['img_url'];
      $bc_text = $bc_label['default_text'];
      $bc_image = array(
        '#theme' => 'image',
        '#path' => $bc_image_url,
        '#alt' => $bc_text,
        '#height' => $height,
        '#title' => $label_type,
      );
      $bc_markup = '<div class="tk_icon"">' . drupal_render($bc_image) . '</div><div class="lc_textblock" style="display:none"><div class="tk_title">' . $label_type . '</div><div class="tk_text">' . $bc_text . '</div></div>';
      $bc_badge = array(
        '#type' => 'markup',
        '#title' => $label_type,
        '#markup' => $bc_markup,
      );
      $badge_array[] = $bc_badge;      
    }
  }
  if (isset($tk_labels)) {
    foreach($tk_labels as $tk_label) {
      $tk_label = get_object_vars($tk_label);
      $label_type = $tk_label['name'];
      $tk_image_url = $tk_label['img_url'];
      $tk_text = $tk_label['default_text'];
      $tk_image = array(
        '#theme' => 'image',
        '#path' => $tk_image_url,
        '#alt' => $tk_text,
        '#height' => $height,
        '#title' => $label_type,
      );
      $tk_markup = '<div class="tk_icon">' . drupal_render($tk_image) . '</div><div class="lc_textblock" style="display:none"><div class="tk_title">' . $label_type . '</div><div class="tk_text">' . $tk_text . '</div></div>';
      $tk_badge = array(
        '#type' => 'markup',
        '#title' => $label_type,
        '#markup' => $tk_markup,
      );
      $badge_array[] = $tk_badge;
    }
  }
  
  if (isset($notice)) {
    return $badge;
  }
  else {
    return $badge_array;
  }
}

