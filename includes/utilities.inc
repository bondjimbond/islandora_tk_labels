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
  $uri_field = variable_get('islandora_tk_labels_rights_field', 'dc.rights');
  $qp->solrParams['fl'] = implode(', ', array(
    'PID',
    $uri_field,
  ));
  $qp->solrStart = 0;
  $qp->solrLimit = 100000;
  $qp->executeQuery(FALSE);
  if ($qp->islandoraSolrResult['response']['numFound'] > 0) {
    if (array_key_exists($uri_field, $qp->islandoraSolrResult['response']['objects']['0']['solr_doc'])) {
      $rights_uri_array = ($qp->islandoraSolrResult['response']['objects']['0']['solr_doc'][$uri_field]);
      foreach ($rights_uri_array as $uri) {
        if (strpos($uri, 'localcontexts.org')) {
          $rights_uri = $uri;
        }
      }
    }
  }
  if (!isset($rights_uri)) {
    return;
  }
  else {
    return $rights_uri;
  }
}

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


/**
 * Take URI, return HTML code for a rights statement badge.
 * @return string
 */
function islandora_tk_labels_get_html($uri) {
  $height = variable_get('islandora_tk_labels_image_height', '31');
  $token = islandora_tk_labels_authenticate();
  $token = 'Bearer ' . $token;
  // Extract the relevant info from the URI and translate to an API URI.
  $community = "demo-community";
  $class = "TK";
  $code = explode("/", $uri);
  end($code);
  $code = strtoupper(prev($code));
  $base_url = "https://api.localcontexts.org/v0";
  $request_url = $base_url . "/labels/" . $community . "/" . $class . "/" . $code;

  $options = array(
    'method' => 'GET',
    'headers' => array(
      'Accept' => 'application/json',
      'Authorization' => $token,
    ),
  );

  // The result has lots of objects that must be turned into arrays.
  $result = get_object_vars(drupal_http_request($request_url, $options));
  $result = get_object_vars(json_decode($result['data']));
  
 // Using $result gets you the result for a specific community. Currently using the demo community, which is California.
 // Using $result_template gets you the generic information, so using that until we learn more about how communities work.

  $result_template = get_object_vars($result['template']);

  $title = $result_template['title'];
  $description = $result_template['description'];

  $image_array = get_object_vars($result_template['icon']);
  $image_path = $base_url . $image_array['url'];

  $alt_text = '<p>' . $title . '</p><p>' . $description . '</p>';

  $img = array(
    '#theme' => 'image',
    '#path' => $image_path,
    '#alt' => $alt_text,
    '#height' => $height,
    '#title' => $title,
  );

  $badge = array(
    '#type' => 'link',
    '#title' => drupal_render($img),
    '#href' => $uri,
    '#options' => array(
      'attributes' => array(
        'target' => '_blank',
      ),
      'html' => TRUE,
    ),
  );
  return $badge;
}

