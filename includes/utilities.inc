?php

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
      'Accept' => 'application/json',
      'Content-Type' => 'application/json',
    ),
    'data' => array(
       'identifier' => $user,
       'password' => $password,
    ),
  );

  $token = drupal_http_request($url, $options);
dd($token);
  return($token);
}


/**
 * Take URI, return HTML code for a rights statement badge.
 * @return string
 */
function islandora_tk_labels_get_html($uri) {
  $height = variable_get('islandora_tk_labels_image_height', '31');
  $token = islandora_tk_labels_authenticate();

  // Extract the relevant info from the URI and translate to an API URI.
  $community = "demo-community";
  $class = "tk";
  $code = explode($uri, "/");
  end($code);
  $code = prev($code);


  $options = array(
    'method' => 'GET',
    'headers' => array(
      'Accept' => 'application/json',
      'Authorization' => $token,
    ),
  );

  // The result has lots of objects that must be turned into arrays.

  $result = get_object_vars(drupal_http_request($uri, $options));
/*
  $result_redirect = drupal_http_request('http://rightsstatements.org' . $result['headers']['location'], $options);
  $result_json = get_object_vars($result_redirect);
  $result_json = get_object_vars(json_decode($result_json['data']));

  $definitions = $result_json['definition'];

  $preferred_language = !empty($_SERVER['HTTP_ACCEPT_LANGUAGE']) ? strtok(strip_tags($_SERVER['HTTP_ACCEPT_LANGUAGE']), ',') : '';
  $preferred_language = substr($preferred_language, 0, 2);
  $possible_languages = ['de', 'et', 'fi', 'fr', 'pl', 'en', 'sv-fi', 'es'];
  if (in_array($preferred_language, $possible_languages, true)) {
    $language = $preferred_language;
  }
  else {
    $language = 'en';
  }

  foreach ($definitions AS $definition) {
    $definition = get_object_vars($definition);
    if ($definition['@language'] == $language) {
      $definition_value = $definition['@value'];
    }
  }

  $notes = $result_json['note'];

  foreach ($notes AS $note) {
    $note = get_object_vars($note);
    if ($note['@language'] == $language) {
      $note_value = $note['@value'];
    }
  }

  $titles = $result_json['prefLabel'];

  foreach ($titles AS $title) {
    $title = get_object_vars($title);
    if ($title['@language'] == $language) {
      $title_value = $title['@value'];
    }
  }
  $alt_text = $title_value . '<p>' . $definition_value . '</p><p>' . $note_value . '</p>';
*/
dd($result);
  $img = array(
    '#theme' => 'image',
    '#path' => $image,
    '#alt' => $alt_text,
    '#height' => $height,
    '#title' => $title_value,
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

