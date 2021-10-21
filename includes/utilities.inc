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

/* Authentication may not be necessary anymore?

function islandora_tk_labels_authenticate() {
  $url = "https://api.localcontexts.org/v0/auth/local";
  $user = variable_get('islandora_tk_labels_user');
  $password = variable_get('islandora_tk_labels_password');
  $options = array(
    'method' => 'POST',
    'headers' => array(
      'Content-Type' => 'application/json',
      'Accept' => 'application/json',
    ),
    'data' => json_encode(array(
       'identifier' => $user,
       'password' => $password,
    )),
  );

  $response = drupal_http_request($url, $options);
  $response = json_decode(json_encode($response), true)['data'];
  $response = explode(",", $response);
  foreach ($response AS $key) {
    if (strpos($key, 'jwt') !== false) {
      $token_key = $key;
    }
  }
  $token_array = explode(":", $token_key);
  $token = str_replace('"', '', $token_array[1]);

  return($token);
}
*/

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
  $result = get_object_vars(json_decode($result['data']));
  $notice = get_object_vars($result['notice'][0]);
  $project_title = $result['title'];
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
  $bc_markup = '<div class="bc_icon">' . drupal_render($bc_image) . '</div><div class="bc_text">' . $bc_text . '</div>';
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
  $tk_markup = '<div class="tk_icon">' . drupal_render($tk_image) . '</div><div class="tk_text">' . $tk_text . '</div>';
  $tk_badge = array(
    '#type' => 'markup',
    '#title' => 'Traditional Knowledge Label',
    '#markup' => $tk_markup,                                                                                              
  );

  if ($title == "biocultural_and_traditional_knowledge") {
    $badge = array(
      $bc_badge,
      $tk_badge,
      );
  }
  elseif ($title == "traditional_knowledge") {
    $badge = $tk_badge;
  }
  elseif ($title == "biocultural") {
    $badge = $bc_badge;
  }
  return $badge;
}

